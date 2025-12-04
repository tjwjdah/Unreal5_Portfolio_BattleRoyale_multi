// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "LobbyPlayerController.h"
#include "../BRGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(ALobbyGameState, LobbyPlayers);

}

void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALobbyGameState::UpdatePings,3.f, true);
	}
}

void ALobbyGameState::UpdatePings()
{
	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController())return;
	RemoveIndexes.Empty();
	for (int32 i = 0; i < LobbyPlayers.Num(); i++) {
		if (IsValid(LobbyPlayers[i].PC)) {
			LobbyPlayers[i].Ping = LobbyPlayers[i].PC->GetPlayerState<APlayerState>()->GetPingInMilliseconds();
			CV(UBRGameInstance, Instance, GetGameInstance());
			if (Instance)
				Instance->UpdateEntryPing(LobbyPlayers[i].ID, LobbyPlayers[i].Ping);
		}
		else {
			RemoveIndexes.Add(i);
		}
	}
	for (int32 i = RemoveIndexes.Num() - 1; i >= 0; i--)
	{
		CV(UBRGameInstance, Instance, GetGameInstance());
		if (Instance)
			Instance->RemoveEntry(LobbyPlayers[RemoveIndexes[i]].ID);
		LobbyPlayers.RemoveAt(RemoveIndexes[i]);
		
	}
}

void ALobbyGameState::OnRep_LobbyPlayers()
{
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController()) {
		CV(ALobbyPlayerController, Controller, UGameplayStatics::GetPlayerController(GetWorld(), 0));
		Controller->AddLobbyPlayers(LobbyPlayers);
	}
}
