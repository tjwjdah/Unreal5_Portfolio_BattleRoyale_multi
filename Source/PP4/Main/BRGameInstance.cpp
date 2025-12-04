// Fill out your copyright notice in the Description page of Project Settings.


#include "BRGameInstance.h"
#include "../Widget/Lobby/LobbyScreen.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Online/OnlineSessionNames.h"



UBRGameInstance::UBRGameInstance() {
	ServerSettings.GameMode = EGameModeList::Solo;
	ServerSettings.MaxPlayersRequired_Start = 2.f;
	ServerSettings.MinimumPlayersRequired_Start = 2.f;
	ServerSettings.ReadyUpTime = 10.f;
	ServerSettings.MaxWaitTimeForceStartWithMinimumPlayers = 10.f;
	ServerSettings.FillWithBotsOnMaxWaitTime = true;
	ServerSettings.FriendlyFire = false;
	ServerSettings.SquadPlayersCount = 5.f;
	ServerSettings.RequestSpawnWithLobbyCharacter = true;
	ServerSettings.IsDeathMatch = false;
	LobbyMap = TEXT("LobbyMap");
	PathMap = TEXT("/Game/Assetss/1-Maps/");
	GameMap = TEXT("MapWithAirplane/PlaneMainMap");
	CharacterID = -1;
	DesiredServerName = TEXT("DefaultServerName");


	static ConstructorHelpers::FObjectFinder<UDataTable>	PickupDataInfo(TEXT("/Game/Item/PickupDataInfo.PickupDataInfo"));

	if (PickupDataInfo.Succeeded())
		PickupsDataTable = PickupDataInfo.Object;

	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacter"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter1(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild1"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter2(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild2"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter3(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild3"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter4(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild4"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter5(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild5"));

	if (LobbyCharacter.Succeeded() && LobbyCharacter1.Succeeded() && LobbyCharacter2.Succeeded() && LobbyCharacter3.Succeeded() && LobbyCharacter4.Succeeded() && LobbyCharacter5.Succeeded())
	{
		LobbyCharacters.Add(LobbyCharacter.Class);
		LobbyCharacters.Add(LobbyCharacter1.Class);
		LobbyCharacters.Add(LobbyCharacter2.Class);
		LobbyCharacters.Add(LobbyCharacter3.Class);
		LobbyCharacters.Add(LobbyCharacter4.Class);
		LobbyCharacters.Add(LobbyCharacter5.Class);
	}

	static ConstructorHelpers::FClassFinder<ABRCharacter> Character1(TEXT("/Game/Characters/SubCharacters/BPBRCharacter1"));
	static ConstructorHelpers::FClassFinder<ABRCharacter> Character2(TEXT("/Game/Characters/SubCharacters/BPBRCharacter2"));
	static ConstructorHelpers::FClassFinder<ABRCharacter> Character3(TEXT("/Game/Characters/SubCharacters/BPBRCharacter3"));
	static ConstructorHelpers::FClassFinder<ABRCharacter> Character4(TEXT("/Game/Characters/SubCharacters/BPBRCharacter4"));
	static ConstructorHelpers::FClassFinder<ABRCharacter> Character5(TEXT("/Game/Characters/SubCharacters/BPBRCharacter5"));
	static ConstructorHelpers::FClassFinder<ABRCharacter> Character6(TEXT("/Game/Characters/SubCharacters/BPBRCharacter6"));

	if (Character1.Succeeded() && Character2.Succeeded() && Character3.Succeeded() && Character4.Succeeded() && Character5.Succeeded() && Character6.Succeeded())
	{
		MainCharacters.Add(Character1.Class);
		MainCharacters.Add(Character2.Class);
		MainCharacters.Add(Character3.Class);
		MainCharacters.Add(Character4.Class);
		MainCharacters.Add(Character5.Class);
		MainCharacters.Add(Character6.Class);
	}
	static ConstructorHelpers::FClassFinder<ULobbyScreen> LobbyW(TEXT("/Game/Widget/Lobby/BPLobbyScreen"));

	if (LobbyW.Succeeded())
		LobbyWidgetClass = LobbyW.Class;

	
}
void UBRGameInstance::Init()
{
	Super::Init();
	if (!SessionInterface.IsValid()) {
		if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get()) {
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface.IsValid()) {
				SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBRGameInstance::OnCreateSessionComplete);
				SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBRGameInstance::OnDestroySessionComplete);
				SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBRGameInstance::OnFindSessionsComplete);
				SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBRGameInstance::OnJoinSessionComplete);
			}
		}
	}

}
void UBRGameInstance::ChangeLobbyClass(int32 _ID)
{
	if (_ID < LobbyCharacters.Num() && _ID >= 0) {
		CharacterID = _ID;
		if (IsValid(LobyCharacterPreview)) {
			LobyCharacterPreview->Destroy();
		}
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ALobbyCharacter* Character = GetWorld()->SpawnActor<ALobbyCharacter>(LobbyCharacters[CharacterID],
			FoundActors[0]->GetActorTransform(),
			param);
		if (Character) {
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Character);
			LobyCharacterPreview = Character;
			ServerSettings.RequestSpawnWithLobbyCharacter = true;
		}

	}
}

bool UBRGameInstance::IsDeathMatchGame()
{
	return ServerSettings.IsDeathMatch;
}

void UBRGameInstance::DEDICATED_RefreshMap()
{
	if (IsRunningDedicatedServer())
	{
		GetWorld()->ServerTravel(PathMap + GameMap + TEXT("?listen?Port=7777"));
	}
}

void UBRGameInstance::ShowLobbyWidget(APlayerController* _PlayerC)
{
	//로비 위젯 만들기

	LobbyWidget = Cast<ULobbyScreen>(CreateWidget(GetWorld(), LobbyWidgetClass));
	LobbyWidget->SetPlayerC(_PlayerC);
	LobbyWidget->SetIsListen(IsListen);
	LobbyWidget->AddToViewport();
}

void UBRGameInstance::HostGame(APlayerController* _PlayerController, int32 _PublicConnections, bool _bUseLAN, int32 _Mode, int32 _MaxPlayersRequired_Start, int32 _MinimumPlayersRequired_Start, bool _FillWithBotsOnMaxWaitTime, bool _FriendlyFire, FString _MapName, bool _OpenMap, bool _IsDeathMatch, FString _ServerName)
{
	//Session setup
	GameMap = _MapName;
	ServerSettings.GameMode = (EGameModeList)_Mode;
	ServerSettings.MaxPlayersRequired_Start = _MaxPlayersRequired_Start;
	ServerSettings.MinimumPlayersRequired_Start = _MinimumPlayersRequired_Start;
	ServerSettings.FillWithBotsOnMaxWaitTime = _FillWithBotsOnMaxWaitTime;
	ServerSettings.FriendlyFire = _FriendlyFire;
	ServerSettings.IsDeathMatch = _IsDeathMatch;
	MaxPlayers = _PublicConnections;
	OpenMap = _OpenMap;
	if (_PublicConnections > 0) {
		IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
		if (Subsystem)
		{
			SessionInterface = Subsystem->GetSessionInterface();
			if (SessionInterface.IsValid())
			{
				FOnlineSessionSettings SessionSetting;
				SessionSetting.bIsLANMatch = _bUseLAN;
				SessionSetting.NumPublicConnections = _PublicConnections; //최대 인원 수
				SessionSetting.bShouldAdvertise = true; //찾을 수 있게 광고할지(검색 가능하게 노출할지)
				SessionSetting.bUsesPresence = true; //친구 목록 기반 검색(친구가 있는 방 찾기) 같은 걸 지원하는 서브시스템에서 친구 세션 검색 가능.
				if (_ServerName != TEXT(""))
					DesiredServerName = _ServerName;
				SessionSetting.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, 
					EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
				SessionSetting.Set(TEXT("CURRENT_PLAYERS"), 1, EOnlineDataAdvertisementType::ViaOnlineService);
				PendingSessionSettings = SessionSetting;
				PendingControllerId = _PlayerController->GetLocalPlayer()->GetControllerId();
				auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
				if (ExistingSession != nullptr)
					SessionInterface->DestroySession(SESSION_NAME);
				else
					CreateSessionInternal(PendingControllerId, SessionSetting);
			}
		}
	}
}

void UBRGameInstance::CreateSessionInternal(int32 ControllerId,FOnlineSessionSettings SessionSettings)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->CreateSession(ControllerId, SESSION_NAME, SessionSettings);
	}
}


void UBRGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
		IsListen = true;
		FString Map = OpenMap ? GameMap : LobbyMap;
		UGameplayStatics::OpenLevel(this, FName(*(PathMap + Map)), true, TEXT("?listen"));

		if(LobbyWidget)
		LobbyWidget->HostSuccess();		
	}
	else {
		LobbyWidget->HostFailed();
		MaxPlayers = 0;
	}
}

void UBRGameInstance::GoBackLobby()
{
	IsListen = false;
	DestroySession();
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController)
		PlayerController->ClientTravel(LobbyMap, ETravelType::TRAVEL_Absolute);
}

void UBRGameInstance::DestroySession()
{
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
}

void UBRGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success && PendingSessionSettings.IsSet())
	{
		CreateSessionInternal(PendingControllerId, PendingSessionSettings.GetValue());
		PendingSessionSettings.Reset();
	}
	
}

void UBRGameInstance::FindSession(APlayerController* _PlayerController,int32 _MaxResults,bool _UseLan)
{
	if (!SessionInterface.IsValid()) return;
	SessionSearch = MakeShared<FOnlineSessionSearch>();
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = _UseLan;
		SessionSearch->MaxSearchResults = _MaxResults;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(_PlayerController->GetLocalPlayer()->GetControllerId(), SessionSearch.ToSharedRef());
	}
}

void UBRGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		LobbyWidget->FindSessionSuccess(SessionSearch->SearchResults);
	}
	else {
		LobbyWidget->FindSessionFailed();
	}
}

void UBRGameInstance::Join(APlayerController* _PlayerController,const FOnlineSessionSearchResult& _Result)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	SessionInterface->JoinSession(_PlayerController->GetLocalPlayer()->GetControllerId(), SESSION_NAME, _Result);
}

void UBRGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address, NAME_GamePort)) {
		LobbyWidget->JoinSessionFailed();
		return;
	}
	
	LobbyWidget->JoinSessionSucess();
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) 
		return;
	ENetMode NM = PlayerController->GetWorld()->GetNetMode();
	if (NM == NM_Client || NM == NM_Standalone) {
		if (Address.EndsWith(":0"))
		{
			Address.RemoveFromEnd(TEXT(":0"));
			Address += TEXT(":7777");
		}
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}


void UBRGameInstance::AddPlayerEntryLobby(int32 _ID, FText _PlayerNameVar, FText _PlayerPingVar)
{
	//로비에서 플레이어 컨트롤러가 호출하는 플레이어 목록에 플레이어 항목 추가
	if (IsValid(LobbyWidget)) {
		LobbyWidget->AddPlayerEntry(_ID, _PlayerNameVar, _PlayerPingVar);
	}
}

void UBRGameInstance::RemoveEntry(int32 _ID)
{
	if (IsValid(LobbyWidget)) {
		LobbyWidget->RemoveEntry(_ID);
	}
}

void UBRGameInstance::UpdateEntryPing(int32 _ID, int32 _Ping)
{
	if (IsValid(LobbyWidget)) {
		LobbyWidget->UpdateEntryPing(_ID, _Ping);
	}
}


void UBRGameInstance::SetPlayerName(FText _InText)
{
	if (IsValid(LobbyWidget)) {
		FText Name = (CustomCharName == "") ? _InText : FText::FromString(CustomCharName);
		LobbyWidget->SetCharacterName(Name);
	}
}

void UBRGameInstance::LoadGame(APlayerController* _SpecificPlayer)
{
	//ServerTravel after host is successful

	if (IsValid(LobbyWidget)) {
		LobbyWidget->RemoveFromParent();
	}
	GetWorld()->ServerTravel(PathMap + GameMap + TEXT("?listen?Port=7777"));
}

AZone* UBRGameInstance::GetZone()
{
	return Zone;
}


