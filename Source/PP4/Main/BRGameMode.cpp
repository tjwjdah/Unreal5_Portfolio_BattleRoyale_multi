// Fill out your copyright notice in the Description page of Project Settings.


#include "BRGameMode.h"
#include "BRGameInstance.h"
#include "BRGameState.h"
#include "BRPlayerState.h"
#include "BRSpectator.h"
#include "../Character/AI/AICharacter.h"
#include "../Character/BRCharacter.h"
#include "../map/PlaneDropGameModeComp.h"
#include "BRPlayerController.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"

ABRGameMode::ABRGameMode()
{


	static ConstructorHelpers::FClassFinder<APlayerStart>	StartingZonePlayerStart(TEXT("/Game/World/BPStartingZonePlayerStart"));
	if (StartingZonePlayerStart.Succeeded())
		m_StartingZonePlayerStart = StartingZonePlayerStart.Class;
	
	static ConstructorHelpers::FClassFinder<APlayerStart>	ArenaPlayerStart(TEXT("/Game/World/BPArenaPlayerStart"));
	if (ArenaPlayerStart.Succeeded())
		m_ArenaPlayerStart = ArenaPlayerStart.Class;

	static ConstructorHelpers::FClassFinder<APlayerStart>	AIPlayerStart(TEXT("/Game/World/BPAIPlayerStart"));
	if (AIPlayerStart.Succeeded())
		m_AIPlayerStart = AIPlayerStart.Class;
	
	PlaneDropGameModeComp= CreateDefaultSubobject<UPlaneDropGameModeComp>(TEXT("PlaneDropGameModeComp"));

	PlaneDropGameModeComp->InstantSpawn = true;
	PlaneDropGameModeComp->FirstSpawnDelay = 20.f;
	PlaneDropGameModeComp->PlaneAcceleration = 15000.f;
	PlaneDropGameModeComp->PlaneMaxSpeed = 20000.f;
	PlaneDropGameModeComp->MaxNumPlaneSpawns = 1.f;

	HandleOnPostLogin = true;
	LatestSpawnPoint = -1;
	LatestSpawnPoint2 = -1;
	Colors.Add(FLinearColor(0.0f, 0.061501f, 1.f, 1.f));
	Colors.Add(FLinearColor(1.0f, 0.0f, 0.309596f, 1.f));
	Colors.Add(FLinearColor(1.0f, 0.0f, 0.f, 1.f));
	Colors.Add(FLinearColor(0.01861f, 0.770833f, 0.f, 1.f));
	Colors.Add(FLinearColor(0.067708f, 0.067708f, 0.067708f, 1.f));
	Colors.Add(FLinearColor(1.0f, 0.542251f, 0.f, 1.f));
	Colors.Add(FLinearColor(0.f, 0.485226f, 0.875f, 1.f));
	Colors.Add(FLinearColor(0.f, 0.192708, 0.003542, 1.f));
	Colors.Add(FLinearColor(0.f, 0.f, 0.92636f, 1.f));
	Colors.Add(FLinearColor(0.f, 1.f, 0.794866f, 1.f));
	Colors.Add(FLinearColor(0.200399f, 0.f, 0.692708f, 1.f));

	ConstructorHelpers::FClassFinder<AAICharacter> AIClass1(TEXT("/Game/Characters/AI/BPAICharacter"));
	//ConstructorHelpers::FClassFinder<AAICharacter> AIClass2(TEXT("Blueprint'/Game/Characters/AI/BPPistolAI.BPPistolAI'"));
	//ConstructorHelpers::FClassFinder<AAICharacter> AIClass3(TEXT("Blueprint'/Game/Characters/AI/BPShotGunAI.BPShotGunAI'"));
	//ConstructorHelpers::FClassFinder<AAICharacter> AIClass4(TEXT("Blueprint'/Game/Characters/AI/BPSniperAI.BPSniperAI'"));
	if (AIClass1.Succeeded())
		AIClassArray.Add(AIClass1.Class);
	/*if (AIClass1.Succeeded() && AIClass2.Succeeded() && AIClass3.Succeeded() && AIClass4.Succeeded()) {
		AIClassArray.Add(AIClass1.Class);
		//AIClassArray.Add(AIClass2.Class);
		//AIClassArray.Add(AIClass3.Class);
		//AIClassArray.Add(AIClass4.Class);
	}*/

	bStartPlayersAsSpectators = false;
	static ConstructorHelpers::FClassFinder<ABRCharacter>	Char(TEXT("/Game/Characters/BPBRCharacter"));
		if (Char.Succeeded())
		DefaultPawnClass = Char.Class;
	PlayerControllerClass = ABRPlayerController::StaticClass();
	GameStateClass = ABRGameState::StaticClass();
	PlayerStateClass = ABRPlayerState::StaticClass();
	SpectatorClass = ABRSpectator::StaticClass();
	bUseSeamlessTravel = true;
}

void ABRGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ABRGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetSettings();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRGameMode::ForceStartGame, Settings.MaxWaitTimeForceStartWithMinimumPlayers, false);
}

void ABRGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	//플레이어 연결 종료(끊김)를 처리하라
	ABRPlayerController* Controller = Cast<ABRPlayerController>(Exiting);
	CV(ABRGameState, State, GameState);
	if (IsValid(Controller)) {
		if (State->RemovePlayer(Controller)) {
			if (State->GetGameStatus() == EGameStatus::Starting) {

				if (State->GetPlayerLength() < Settings.MinimumPlayersRequired_Start) {
					State->SetGameStatus(EGameStatus::Waiting);
					auto& PlayerList = State->GetPlayerList();
					for (int32 i = 0; i < PlayerList.Num(); i++) {
						if (IsValid(PlayerList[i])) {
							ABRPlayerController* ABRController = Cast<ABRPlayerController>(PlayerList[i]);
							if(IsValid(ABRController))
								ABRController->UpdateKillFeed(FText::FromString(TEXT("DC")),
								FText::FromString(FString::Printf(TEXT("Player %d"),Controller->GetPlayerState<APlayerState>()->GetPlayerId()))
								, FText::FromString(TEXT("Disconnected")));
						}
					}
				}
			}
			else {
				if (!IsDeathMatch() && State->GetGameStatus() == EGameStatus::Started) {
					//플레이어가 연결이 끊긴 경우(죽은 것이 아님) 승리 조건을 처리하라
					if(Settings.GameMode == EGameModeList::Solo)
					{ 
						HandleSoloWinCondition();
					}
					else {
						HandleTeamWinCondition();
					}
				}
			}
		}
	}

}

void ABRGameMode::PostLogin(APlayerController* NewPlayer)
{

	//PostLogin - 특정 자식 클래스로 스폰하고 싶다면, 월드 세팅(World Settings)에서 해당 클래스를 설정하고, GameInstance의 서버 설정에서 '로비 캐릭터로 스폰'을 비활성화하세요.
	
	GetSettings();
	if (!DefaultPawnClass) {
		DefaultPawnClass = ABRCharacter::StaticClass();
	}
	Super::PostLogin(NewPlayer);
	
}


void ABRGameMode::InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer)
{
	Super::InitStartSpot_Implementation(StartSpot, NewPlayer);
	//BeginPlay는 서버에서 딱 한번 실행되기 때문에 이후 접속하는 플레이어들을 위해 InitStartSpot에서 유사 코드 적용
	GetSettings();
	if (IsGameTime(ForceStart)) {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRGameMode::StartGameSetTimer, 1.f, false);
	}
	
}
//리얼 엔진(UE)에서 Listen Server 환경에서 PostLogin이 BeginPlay보다 먼저 실행되므로, 특정 호출을 PostLogin에서 실행해야 한다

void ABRGameMode::GetSettings()
{
	//게임 인스턴스에서 서버 설정을 가져오기
	if (DoOnce) return;
	DoOnce = true;
	UBRGameInstance* GameInst = Cast<UBRGameInstance>(GetGameInstance());
	Settings = GameInst->GetServerSetting();
	CV(ABRGameState, State, GameState);
	State->SetServerSettings(Settings);
	if (Settings.RequestSpawnWithLobbyCharacter) {
		HandleOnPostLogin = false;
	}
}

void ABRGameMode::ForceStartGame()
{

	//플레이어가 충분하면 게임 시작
	CV(ABRGameState, State, GameState);
	if (State->GetGameStatus() == EGameStatus::Waiting) {
		ForceStart = true;

		if(IsGameTime(true)){
			GetWorldTimerManager().SetTimer(GameCountDownHandle, this, &ABRGameMode::GameCountDown,Settings.ReadyUpTime, false);
			State->SetGameStatus(EGameStatus::Starting);
			CheckFillBots();
		}
		else {
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle,this, &ABRGameMode::ForceStartGame, Settings.MaxWaitTimeForceStartWithMinimumPlayers, false);
		}
	}

}

APawn* ABRGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	if (HandleOnPostLogin)
	{
	
		ABRPlayerController* Controller = Cast<ABRPlayerController>(NewPlayer);
		if (!IsValid(Controller))return nullptr;
		if (!IsGameTime(false)) {
			CV(ABRGameState, State, GameState);
			int32 TeamID = State->AddNewPlayer(Settings.GameMode, Controller);
			ABRCharacter* Character = GetWorld()->SpawnActorDeferred<ABRCharacter>(GetCharacterRandomClass(-1), SpawnTransform, nullptr, nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (IsValid(Character)) {
				Character->SetMarkerColor(GetGoodColor());
				Character->SetTeamID(TeamID);
				Character->FinishSpawning(SpawnTransform);
				Character->SetInstigator(GetInstigator());
				Controller->ShowInGameWidget();
				Controller->SetMyPawnRef(Character);
			
				return Character;
			}
		}
		else {
			Controller->LeaveServer();
		}
	}
	return nullptr;
}

AActor* ABRGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	return  GetRandomSpawnLocation();
}

TSubclassOf<ABRCharacter> ABRGameMode::GetCharacterRandomClass(int32 _CID)
{
	UBRGameInstance* GameInst = Cast<UBRGameInstance>(GetGameInstance());
	TArray<TSubclassOf<ABRCharacter>>& Characters = GameInst->GetMainCharacters();
	int32 GetClassNum = _CID < Characters.Num() && (_CID > -1) ? _CID : UKismetMathLibrary::RandomInteger(Characters.Num());
	return Characters[GetClassNum];
}

APlayerStart* ABRGameMode::GetRandomSpawnLocation()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_StartingZonePlayerStart, FoundActors);
	
	LatestSpawnPoint = (LatestSpawnPoint + 1) >= FoundActors.Num() ? 0 : (LatestSpawnPoint + 1);
	return Cast<APlayerStart>(FoundActors[LatestSpawnPoint]);
}

bool ABRGameMode::IsGameTime(bool _Forced)
{
	int32 Start = _Forced ? Settings.MinimumPlayersRequired_Start : Settings.MaxPlayersRequired_Start;
	
	CV(ABRGameState, State, GameState);

	return State->GetPlayerLength() >= Start;
}

void ABRGameMode::StartGame()
{
	CV(ABRGameState, State, GameState);
	State->SetGameStatus(EGameStatus::Started);
	const TArray<APlayerController*>& PlayerList = State->GetPlayerList();
	for (auto* Player : PlayerList) {
		if(IsValid(Player) && IsValid(Player->GetPawn())) {
			ABRCharacter* Character = Cast<ABRCharacter>(Player->GetPawn());
			if (Character)
			{
				Character->GameStarted(false);
				Character->SetActorTransform(GetRandomSpawnLocation_Arena(),false,nullptr, ETeleportType::TeleportPhysics);
			}
			else 
			{
				ABRPlayerController* PlayerController = Cast<ABRPlayerController>(Player);
				if (IsValid(PlayerController) && PlayerController->GetBRPawn()->GetIsVehicleDriver()) {
					PlayerController->GetBRPawn()->MCPlayerExitedVehicle();
					PlayerController->Possess(PlayerController->GetBRPawn());
				}
				ABRCharacter* Char = Cast<ABRCharacter>(Player->GetPawn());
				if (Char)
				{
					Char->GameStarted(false);
					Char->SetActorTransform(GetRandomSpawnLocation_Arena(), false, nullptr, ETeleportType::TeleportPhysics);
				}
			}
		}
	}
}

void ABRGameMode::UpdateKillFeed(ACharacter* _Killed, ACharacter* _Killer, FString _Source)
{
	if (IsValid(_Killer)) {
		ABRPlayerController* Controller = Cast<ABRPlayerController>(_Killer->GetController());
		if (Controller)
		Controller->ShowKillInfo(GetPlayerName(_Killed));
		
	}
	auto& PlayerList = Cast<ABRGameState>(GameState)->GetPlayerList();
	for (int i = 0; i < PlayerList.Num(); i++) {
		if (!IsValid(PlayerList[i]))continue;
		ABRPlayerController* Controller2 = Cast<ABRPlayerController>(PlayerList[i]);
		if (Controller2)
			Controller2->UpdateKillFeed(GetPlayerName(_Killer), GetPlayerName(_Killed), FText::FromString(_Source));
	}
}

FText ABRGameMode::GetPlayerName(ACharacter* _Player)
{
	if (!IsValid(_Player)) return FText::FromString(TEXT("Unkown"));
	ABRCharacter* Character = Cast<ABRCharacter>(_Player);
	if (Character)
	{
		if (IsValid(_Player->GetController())) {
			ABRPlayerController* Controller = Cast<ABRPlayerController>(_Player->GetController());
			if (Controller) {
				if (Controller->GetCustomIconName() != TEXT(""))
					return FText::FromName(Controller->GetCustomIconName());
			}
		}
		if (Character->GetControllerRef()->GetPlayerState<APlayerState>())
			return FText::FromString(Character->GetControllerRef()->GetPlayerState<APlayerState>()->GetPlayerName());
		else
			return FText::FromString(TEXT("Unkown"));
	}
	else
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(_Player);
		if (AICharacter)
			return FText::FromString(AICharacter->GetAIName());
		else
			return FText::FromString(TEXT("Unkown"));
	}
}

void ABRGameMode::HandleTeamWinCondition()
{
	CV(ABRGameState, State, GameState);
	if (State->GetTeamLength() <= 1 && State->GetAIPlayersCount() == 0) {
		const TArray<APlayerController*>& PlayerList = State->GetPlayerList();
		for (int32 i = 0; i < PlayerList.Num(); i++) {
			ABRPlayerController* Controller = Cast<ABRPlayerController>(PlayerList[i]);
			if (!Controller)return;
				Controller->GameEnded(1);
				ABRCharacter* Character = Cast<ABRCharacter>(Controller->GetPawn());
				if (!Character)return;
					Character->ClientYouWin();

		}
		GameEnded();
	}
}

void ABRGameMode::HandleSoloWinCondition()
{
	CV(ABRGameState, State, GameState);
	
	if (State->GetPlayerLength() == 1) {
		const TArray<APlayerController*>& PlayerList = State->GetPlayerList();
		ABRPlayerController* Controller = Cast<ABRPlayerController>(PlayerList[0]);
		Controller->GameEnded(1);
		ABRCharacter* Character = Cast<ABRCharacter>(Controller->GetPawn());
		if (!Character)return;
		Character->ClientYouWin();
		GameEnded();
	}
	else {
		if (State->GetPlayerLength() == 0)
			GameEnded();
	}
}

FTransform ABRGameMode::GetRandomSpawnLocation_Arena()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_ArenaPlayerStart, FoundActors);
	int32 Len = FoundActors.Num();
	if (Len > 0) {
		LatestSpawnPoint2 = LatestSpawnPoint2 + 1 >= Len ? 0 : LatestSpawnPoint2 + 1;
		return FoundActors[LatestSpawnPoint2]->GetActorTransform();
	}
	else {
		FTransform T;
		T.SetLocation(FVector(0.f, 0.f, 15000.f));
		return T;
	}
}

FLinearColor ABRGameMode::GetGoodColor()
{
	int32 Random =UKismetMathLibrary::RandomInteger(Colors.Num());
	
	return Colors[Random];
}

bool ABRGameMode::IsDeathMatch()
{
	UBRGameInstance* GameInstance = Cast<UBRGameInstance>(GetGameInstance());
	if (!GameInstance)
		return false;
	return GameInstance->IsDeathMatchGame();
}

void ABRGameMode::SpawnAI(bool _CheckGameTime)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), m_AIPlayerStart, FoundActors);
	if (FoundActors.Num() > 0) {
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		GetWorld()->SpawnActor<AAICharacter>(GetRandomAIClass(),
			FoundActors[UKismetMathLibrary::RandomInteger(FoundActors.Num())]->GetActorTransform(),
			param);
		if(_CheckGameTime)StartRetriggerableDelay<ABRGameMode>(&ABRGameMode::CheckIfGameTime, 2.f);
	}
	
}

void ABRGameMode::OnAIDeathCheckWinCondition()
{
	if (Settings.IsDeathMatch) {
		RequestSpawnAI(false);
	}
	else {
		if (Settings.GameMode == EGameModeList::Solo)HandleSoloWinCondition();
		else
			HandleTeamWinCondition();
	}
}

void ABRGameMode::SpawnAIClass(TSubclassOf<AAICharacter> _Class, bool _CheckGameTimer)
{
	
	UClass* BPAIPlayerStart = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/World/BPAIPlayerStart.BPAIPlayerStart_C"));
	if (BPAIPlayerStart)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), BPAIPlayerStart, FoundActors);

		for (AActor* Actor : FoundActors)
		{
			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			GetWorld()->SpawnActor<AAICharacter>(_Class,
				Actor->GetTransform(),
				param);
			if (_CheckGameTimer)StartRetriggerableDelay<ABRGameMode>(&ABRGameMode::CheckIfGameTime, 2.f);
		}
		
	}

}

void ABRGameMode::CheckFillBots()
{
	if (Settings.FillWithBotsOnMaxWaitTime) {
		CV(ABRGameState, State, GameState);
		
		if (State->GetPlayerLength() < Settings.MaxPlayersRequired_Start) {
			int32 LastIndex = Settings.MaxPlayersRequired_Start - State->GetPlayerLength();
			for (int32 i = 1; i <= LastIndex; i++) {
				SpawnAIClass(GetRandomAIClass(),false);
			}
		}
	}
}

TSubclassOf<AAICharacter> ABRGameMode::GetRandomAIClass()
{
	int32 Random = UKismetMathLibrary::RandomInteger(AIClassArray.Num());
	return AIClassArray[Random];
}


void ABRGameMode::CheckIfGameTime()
{
	if (IsGameTime(ForceStart)) {
		if (DoOnce2)return;
		DoOnce2 = true;
		StartGameSetTimer();
	}
}

void ABRGameMode::GameCountDown()
{
	CV(ABRGameState, State, GameState);

	if (State->GetGameStatus() == EGameStatus::Starting) {
		StartGame();
	}
}

void ABRGameMode::StartGameSetTimer()
{

	GetWorldTimerManager().SetTimer(GameCountDownHandle, this, &ABRGameMode::GameCountDown, Settings.ReadyUpTime, false);
	CV(ABRGameState, State, GameState);
	State->SetGameStatus(EGameStatus::Starting);
}

void ABRGameMode::RequestSpawnWithID(int32 ClassID, APlayerController* _Controller)
{
	
	GetSettings();
	if (IsValid(_Controller->GetPawn())) {
	
	}
	else {
		CV(ABRGameState, State, GameState);

		if (State->GetGameStatus() == EGameStatus::Waiting && !IsGameTime(false))
		{
			//Adding the player to a team based on the mode (Solo, Duo etc)
			int32 TeamID= State->AddNewPlayer(Settings.GameMode, _Controller);
			ABRCharacter* Character = GetWorld()->SpawnActorDeferred<ABRCharacter>(GetCharacterRandomClass(ClassID), GetRandomSpawnLocation()->GetActorTransform(), nullptr, nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (IsValid(Character)) {
				Character->SetReplicates(true);
				Character->SetMarkerColor(GetGoodColor());
				Character->SetTeamID(TeamID);
				Character->SetInstigator(GetInstigator());
				Character->FinishSpawning(GetRandomSpawnLocation()->GetActorTransform());
				if (Character->GetController() != nullptr) {
					Character->GetController()->UnPossess(); // 필요 시 해제
				}
				_Controller->Possess(Character);
				CV(ABRPlayerController, PlayerController, _Controller);
				
				if (IsValid(PlayerController)) {
					//플레이어 컨트롤러(클라이언트)에게 게임 내 위젯을 보여달라고 요청합니다. 
					// (시작 플레이에서는 기본적으로 이 작업을 수행할 수 있지만 현재 게임 상태에 대한 중요한 정보를 전달하는 것이 좋습니다.)

					PlayerController->ShowInGameWidget();
					PlayerController->SetMyPawnRef(Character);
					PlayerController->SetTeamID(TeamID);
					if (IsGameTime(ForceStart)) {
						FTimerHandle TimerHandle;
						GetWorldTimerManager().SetTimer(TimerHandle, this, &ABRGameMode::StartGameSetTimer, 1.f, false);
					}
				}
			}
		}
		else {
			//게임이 starting, started or ended되면 연결을 거부하고 플레이어를 킥합니다
			CV(ABRPlayerController, PlayerController, _Controller);
			if (IsValid(PlayerController)) {
				PlayerController->LeaveServer();
			}
		}
	}
}

void ABRGameMode::SpawnOnDeathMatch(APlayerController* _Controller, TSubclassOf<ABRCharacter> _Character, int32 TeamID)
{
	//+5 for destroying character so this is essentially 10 second. (5 then destroy, 5 then respawn)
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, _Controller, _Character, TeamID]() {
		
		FTransform ArenaTransForm = GetRandomSpawnLocation_Arena();

		ABRCharacter* Character = GetWorld()->SpawnActorDeferred<ABRCharacter>(_Character, ArenaTransForm, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if(IsValid(Character)){
		Character->SetMarkerColor(GetGoodColor());
		Character->SetTeamID(TeamID);
		Character->FinishSpawning(ArenaTransForm);
		_Controller->Possess(Character);
		CV(ABRPlayerController, Controller, _Controller);
		Controller->ShowInGameWidget();
		}
		}, 5.f, false);
	
}

void ABRGameMode::RequestSpawnAI(bool _CheckGameTime)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, _CheckGameTime]() {
		SpawnAI(_CheckGameTime);
		}, 5.f, false);
}

void ABRGameMode::PlayerDied(ABRCharacter* _Killed, ACharacter* _Killer, FString _DamageSource)
{
	//플레이어가 사망할 때
	UpdateKillFeed(_Killed, _Killer, _DamageSource);
	CV(ABRGameState, State, GameState);
	
	ABRPlayerController* ControllerRef = _Killed->GetControllerRef();
	if (IsDeathMatch()) {
		ControllerRef->OnDeathMatchDeath();
	}
	else {
		int32 Rank = Settings.GameMode==EGameModeList::Solo ? State->GetPlayerLength() : State->GetTeamLength();
		ControllerRef->GameEnded(Rank);
		State->RemovePlayer(ControllerRef);
	}
	//플레이어가 탈락한 후 승리 조건 처리
	if (IsDeathMatch())return;
	if (Settings.GameMode == EGameModeList::Solo) {
		HandleSoloWinCondition();
	}
	else {
		HandleTeamWinCondition();
	}
}

void ABRGameMode::GameEnded()
{
	CV(ABRGameState, State, GameState);
	State->SetGameStatus(EGameStatus::Ended);
	FTimerHandle TimerHandle;
	//Dedicated Server일 경우, 플레이어들을 로비로 보내고 레벨을 다시 시작함
	GetWorldTimerManager().SetTimer(TimerHandle, [this] {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABRPlayerController::StaticClass(), FoundActors);
		if (IsRunningDedicatedServer()) {
			for (AActor* Actor : FoundActors) {
				CV(ABRPlayerController, Controller, Actor);
				Controller->LeaveServer();
			}
			FTimerHandle TimerHandle2;
			GetWorldTimerManager().SetTimer(TimerHandle2, [this] {
				CV(UBRGameInstance, Instance, GetGameInstance());
				Instance->DEDICATED_RefreshMap();
				}, 5.f, false);
		}
		else {
			for (AActor* Actor : FoundActors) {
				CV(APlayerController, Controller, Actor);
				if (Controller != UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
					CV(ABRPlayerController, Controller1, Controller);
					Controller1->LeaveServer();
				}
			}
			CV(ABRPlayerController, Controller, UGameplayStatics::GetPlayerController(GetWorld(), 0));
				Controller->LeaveServer();
		}
		
		
		}, 10.f, false);
}

