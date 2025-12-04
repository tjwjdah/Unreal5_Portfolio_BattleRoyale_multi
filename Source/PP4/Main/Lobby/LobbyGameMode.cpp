// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyGameState.h"
#include "LobbyPlayerController.h"
#include "LobbyPlayerState.h"
#include "../../Character/LobbyCharacter/LobbyCharacter.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"


ALobbyGameMode::ALobbyGameMode() {

	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacter"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter1(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild1"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter2(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild2"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter3(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild3"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter4(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild4"));
	static ConstructorHelpers::FClassFinder<ALobbyCharacter> LobbyCharacter5(TEXT("/Game/Characters/LobbyCharacter/BPLobbyCharacterChild5"));

	if (LobbyCharacter.Succeeded() && LobbyCharacter1.Succeeded() && LobbyCharacter2.Succeeded() && LobbyCharacter3.Succeeded() && LobbyCharacter4.Succeeded() && LobbyCharacter5.Succeeded())
	{
		LobbyCharacterClass.Add(LobbyCharacter.Class);
		LobbyCharacterClass.Add(LobbyCharacter1.Class);
		LobbyCharacterClass.Add(LobbyCharacter2.Class);
		LobbyCharacterClass.Add(LobbyCharacter3.Class);
		LobbyCharacterClass.Add(LobbyCharacter4.Class);
		LobbyCharacterClass.Add(LobbyCharacter5.Class);
	}
	
	bStartPlayersAsSpectators = false;
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	GameStateClass = ALobbyGameState::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
	bUseSeamlessTravel = true;
}
void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

//서버에서 어떤 플레이어가 "정상적으로 접속에 성공해서 PlayerController까지 만들어진 직후" 호출
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PrintViewport(1.f, FColor::Red, "PostLogin");
	CV(ALobbyGameState, State, GetWorld()->GetGameState());
	if (State) {
		auto& LobbyPlayers = State->GetLobbyPlayers();
		FLobbyPlayerInfo PlayerInfo;
		APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
		PlayerInfo.Name=PlayerState->GetPlayerName();
		PlayerInfo.ID = PlayerState->GetPlayerId();
		PlayerInfo.Ping = PlayerState->GetPingInMilliseconds();
		PlayerInfo.PC = NewPlayer;
		LobbyPlayers.Add(PlayerInfo);
		if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController()) {
			CV(ALobbyPlayerController, Controller, UGameplayStatics::GetPlayerController(GetWorld(), 0));
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this,Controller, LobbyPlayers]() {
				if(IsValid(Controller))
				Controller->AddLobbyPlayers(LobbyPlayers);
				}, 0.5f, false);
		}
	}

}
