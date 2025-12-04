// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../BRGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(ALobbyPlayerController, ClassID);

}


ALobbyPlayerController::ALobbyPlayerController()
{
	bShowMouseCursor = true;
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController()) {
		CV(UBRGameInstance, Instance, GetGameInstance());
		if (Instance) {
			Instance->ShowLobbyWidget(this);
			ifv(PlayerState) {
				Instance->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
			}
		}
	}
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);/*
	CV(UBRGameInstance, Instance, GetGameInstance());
	if (Instance) {
		//Instance->DestroySession();
	}*/
}

void ALobbyPlayerController::PostSeamlessTravel()
{	
	Super::PostSeamlessTravel();
	/*
	CV(UBRGameInstance, Instance, GetGameInstance());
	if (Instance) {
		//Instance->DestroySession();
	}
	*/
}


void ALobbyPlayerController::AddEntryLoby(int32 _ID, FText _PlayerNameVar, FText _PlayerPingVar)
{
	CV(UBRGameInstance, Instance, GetGameInstance());
	if (Instance) {
		Instance->AddPlayerEntryLobby(_ID, _PlayerNameVar, _PlayerPingVar);
	}
}


void ALobbyPlayerController::AddLobbyPlayers(const TArray<FLobbyPlayerInfo>& _LobbyPlayers)
{
	//로비 플레이어 복제 후 게임 상태에서 호출됨 (RepNotify)
	for (int32 i = 0; i < _LobbyPlayers.Num(); i++) {
		if (AddedPlayers.Contains(_LobbyPlayers[i].ID)) {
			if (*AddedPlayers.Find(_LobbyPlayers[i].ID) != _LobbyPlayers[i].Ping) {
				CV(UBRGameInstance, Instance, GetGameInstance());
				Instance->UpdateEntryPing(_LobbyPlayers[i].ID, _LobbyPlayers[i].Ping);
			}
		}
		else {
			AddedPlayers.Add({ _LobbyPlayers[i].ID,_LobbyPlayers[i].Ping });
			FString String = "Ping(" + FString::FromInt(_LobbyPlayers[i].Ping) + ")";

			AddEntryLoby(_LobbyPlayers[i].ID, FText::FromString(_LobbyPlayers[i].Name), FText::FromString(String));
		}
		
	}

}
