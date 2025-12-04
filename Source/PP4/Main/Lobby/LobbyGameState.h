// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_LobbyPlayers")
	TArray<FLobbyPlayerInfo> LobbyPlayers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<int32> RemoveIndexes;

public:
	virtual void BeginPlay() override;

	void UpdatePings();


public:
	UFUNCTION()
	void OnRep_LobbyPlayers();

	TArray<FLobbyPlayerInfo>& GetLobbyPlayers() { return LobbyPlayers; }
};
