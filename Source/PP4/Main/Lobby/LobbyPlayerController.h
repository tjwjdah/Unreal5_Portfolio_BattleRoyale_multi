// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	TMap<int32, int32> AddedPlayers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	int32 ClassID;
public:
	ALobbyPlayerController();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostSeamlessTravel();


	void AddEntryLoby(int32 _ID, FText _PlayerNameVar, FText _PlayerPingVar);

	void AddLobbyPlayers(const TArray<FLobbyPlayerInfo>& _LobbyPlayers);

};
