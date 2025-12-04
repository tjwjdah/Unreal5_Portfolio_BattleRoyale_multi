// Fill out your copyright notice in the Description page of Project Settings.


#include "BRGameState.h"
#include "BRGameInstance.h"
#include "BRPlayerController.h"
#include "../Widget/InGameWidget.h"
#include "../World/Zone.h"
#include "Net/UnrealNetwork.h"

ABRGameState::ABRGameState()
{
	GameStatus = EGameStatus::Waiting;
	GameMode = EGameModeList::Solo;
	CurrentTeamID = -1;
	ServerWorldTimeSecondsUpdateFrequency = 0.1f;
}

void ABRGameState::GetLifetimeReplicatedProps
(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 서버가 관리하고, 모든 클라이언트와 공유해야 하는 데이터들
	DOREPLIFETIME(ABRGameState, PlayerList); 
	DOREPLIFETIME(ABRGameState, PlayerAIList);
	DOREPLIFETIME(ABRGameState, GameStatus);
	DOREPLIFETIME(ABRGameState, ServerSettings);
	DOREPLIFETIME(ABRGameState, GameMode);
	DOREPLIFETIME(ABRGameState, CanJumpOffPlane);
}


bool ABRGameState::RemovePlayer(APlayerController* _Player)
{
	bool Removed;
	bool RemoveTeam;
	int32 TeamID = Cast<ABRPlayerController>(_Player)->GetTeamID();
	if (!IsValid(_Player))	return false;
	for (int32 i = 0; i < Teams.Num(); i++) {
		CurrentIndex = i;
		if (Teams[i].TeamID == TeamID) {
			Removed = Teams[i].Players.Remove(_Player) >0 ? true :false;
			RemoveTeam = Teams[i].Players.Num() == 0;
			if (RemoveTeam) {
				Teams.RemoveAt(CurrentIndex);
				break;
			}
		}
	}
	if(!RemoveTeam && Removed)
	{
		//우리가 아직 기다리고 있는데 누군가 연결이 끊겼다면, 이 팀은 더 많은 선수들이 필요합니다
		bool Remove = PlayerList.Remove(_Player)>0;
		if (Remove && HasAuthority()) {
				PlayerAliveUpdated(GetPlayerLength());
		}
		bool Check1 = GameStatus == EGameStatus::Waiting || (GameStatus == EGameStatus::Starting && PlayerList.Num() < ServerSettings.MinimumPlayersRequired_Start);
		bool Check2 = Teams[CurrentIndex].Players.Num() < (ServerSettings.GameMode == EGameModeList::Duo ? 2 : ServerSettings.SquadPlayersCount);
		if (Check1 && Check2)
		{
			//더 많은 플레이어가 필요하므로 배열의 끝에 이 팀을 배치하세요.
			auto Team = Teams[CurrentIndex];
			Teams.RemoveAt(CurrentIndex);
			Teams.Add(Team);
		}
		return Remove;
	}


	Removed = PlayerList.Remove(_Player) >0 ;
	if (Removed && HasAuthority()) {
		PlayerAliveUpdated(GetPlayerLength());
	}
	return Removed;
}


void ABRGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) {
		UBRGameInstance* Instance = Cast<UBRGameInstance>(GetGameInstance());
		GameMode = Instance->GetServerSetting().GameMode;
	}
}
void ABRGameState::AddPlayer(APlayerController* _Player)
{
	PlayerList.Add(_Player);
	if (HasAuthority()) {
		PlayerAliveUpdated(GetPlayerLength());
	}
}

int32 ABRGameState::AddNewPlayer(EGameModeList _GameMode, APlayerController* _Controller)
{
	AddPlayer(_Controller);
	if (_GameMode == EGameModeList::Solo) {
		return -1;
	}
	else
	{
		if (Teams.Num() > 0) {
			//마지막 팀이 꽉 찼거나 아직 팀이 없는 경우 새 팀 만들기
			int32 PlayersCount = _GameMode==EGameModeList::Duo ? 2 : ServerSettings.SquadPlayersCount;
			if (PlayersCount == Teams[Teams.Num() - 1].Players.Num()) {
				CurrentTeamID = CurrentTeamID + 1;
				TArray<APlayerController*> NewTeam;
				NewTeam.Add(_Controller);
				Teams.Add(FTeamInfo(CurrentTeamID, NewTeam));
				return CurrentTeamID;
			}
			else {
				//이미 만들어진 팀에 플레이어를 추가합니다 
				for (auto& Player : Teams[Teams.Num() - 1].Players) {
					ABRPlayerController* BRPlayerController1 = Cast<ABRPlayerController>(Player);
					ABRPlayerController* BRPlayerController2 = Cast<ABRPlayerController>(_Controller);
					if (BRPlayerController1 && BRPlayerController2) {
						BRPlayerController1->AddNewTeammate(_Controller);
						BRPlayerController2->AddNewTeammate(Player);
					}
				}
				Teams[Teams.Num() - 1].Players.Add(_Controller);
				return Teams[Teams.Num() - 1].TeamID;
			}
		}
		else 
		{
			CurrentTeamID = CurrentTeamID + 1;
			TArray<APlayerController*> NewTeam;
			NewTeam.Add(_Controller);
			Teams.Add(FTeamInfo(CurrentTeamID, NewTeam));
			return CurrentTeamID;
		}
	}
}
void ABRGameState::AddAIController(AController* _Controller)
{
	PlayerAIList.Add(_Controller);
	PlayerAliveUpdated(GetPlayerLength());
}
bool ABRGameState::GameStarted()
{
	return  GameStatus == EGameStatus::Started;
}
void ABRGameState::RemoveAIController(AController* _Controller)
{
	PlayerAIList.Remove(_Controller);
	PlayerAliveUpdated(GetPlayerLength());
}

void ABRGameState::SetGameStatus(EGameStatus _Status)
{
	if (GameStatus != _Status) {
		GameStatus = _Status;
		if (HasAuthority())
			HandleGameStatusChanged();
	}
}


void ABRGameState::SetCanJumpOffPlane(bool _CanJumpOffPlane)
{
	if (CanJumpOffPlane != _CanJumpOffPlane) {
		CanJumpOffPlane = _CanJumpOffPlane;
		if (HasAuthority())
			HandleCanJumpOffPlaneChanged();
	}
}

void ABRGameState::OnRep_PlayerList()
{
	PlayerAliveUpdated(GetPlayerLength());
}

void ABRGameState::PlayerAliveUpdated(int32 _Alive)
{
	ABRPlayerController* Controller = Cast<ABRPlayerController>
		(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Controller && IsValid(Controller->GetInGameWidget())) {
		Controller->GetInGameWidget()->UpdatePlayersAlive(_Alive);
	}
}

void ABRGameState::OnRep_PlayerAIList()
{
	PlayerAliveUpdated(GetPlayerLength());
}

void ABRGameState::OnRep_GameStatus()
{
	HandleGameStatusChanged();
}

void ABRGameState::HandleGameStatusChanged()
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController()) {
		ABRPlayerController* Controller = Cast<ABRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (!IsValid(Controller))return;
		if (GameStatus == EGameStatus::Starting || GameStatus == EGameStatus::Waiting) {
			Controller->GameStarting(ServerSettings.ReadyUpTime, GameStatus != EGameStatus::Waiting);
		}
		else {
			if (GameStatus == EGameStatus::Started && IsValid(Controller->GetInGameWidget())) {
				Controller->GetInGameWidget()->GameStarted();
				Controller->StopTeammatesAddHandle();
			}
		}
	}

	if (GameStatus == EGameStatus::Started && IsValid(Zone)) {
		
		Zone->StartZone();
	}
}

void ABRGameState::OnRep_CanJumpOffPlane()
{
	HandleCanJumpOffPlaneChanged();
}

void ABRGameState::HandleCanJumpOffPlaneChanged()
{
	if (CanJumpOffPlane && UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController()) {
		ABRPlayerController* Controller = Cast<ABRPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (Controller)
			Controller->CanJumpOff();
	}
}
