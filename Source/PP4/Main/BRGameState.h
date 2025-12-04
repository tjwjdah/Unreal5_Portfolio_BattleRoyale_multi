// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/GameStateBase.h"
#include "BRGameState.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABRGameState : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	ABRGameState();

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_PlayerList")
	TArray<TObjectPtr<APlayerController>> PlayerList;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_PlayerAIList")
	TArray<TObjectPtr<AController>> PlayerAIList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_GameStatus")
	TEnumAsByte<EGameStatus> GameStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	FGameModeSettings ServerSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), ReplicatedUsing = "OnRep_CanJumpOffPlane")
	bool CanJumpOffPlane;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FTeamInfo> Teams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CurrentTeamID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CurrentIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
	 meta = (AllowPrivateAccess = "true"), Replicated)
	TEnumAsByte<EGameModeList> GameMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Found;

	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AZone> Zone;

public:
	virtual void BeginPlay() override;
	


	void AddPlayer(APlayerController* _Player);

	bool RemovePlayer(APlayerController* _Player);

	int32 GetPlayerLength() { return PlayerList.Num() + PlayerAIList.Num(); }

	int32 AddNewPlayer(EGameModeList _GameMode, APlayerController* _Controller);

	bool IsFriendlyFire() { return ServerSettings.FriendlyFire;}

	EGameModeList GetBRGameMode() { return ServerSettings.GameMode; }

	int32 GetTeamLength() { return Teams.Num(); }

	void AddAIController(AController* _Controller);

	bool GameStarted();

	void RemoveAIController(AController* _Controller);

	int32 GetAIPlayersCount() { return PlayerAIList.Num(); }

	void SetServerSettings(FGameModeSettings _Settings) { ServerSettings = _Settings; }

	void SetGameStatus(EGameStatus _Status);

	EGameStatus GetGameStatus() { return GameStatus; }
	
	const TArray<TObjectPtr<APlayerController>>& GetPlayerList() { return PlayerList; }
	const TArray<TObjectPtr<AController>>& GetPlayerAIList() { return PlayerAIList; }

	void PlayerAliveUpdated(int32 _Alive);

	void SetZone(AZone* _Zone) { Zone = _Zone; }

	bool GetCanJumpOffPlane() { return CanJumpOffPlane; }

	void SetCanJumpOffPlane(bool _CanJumpOffPlane);
private:
	UFUNCTION()
	void OnRep_PlayerList();

	UFUNCTION()
	void OnRep_PlayerAIList();
	
	UFUNCTION()
	void OnRep_GameStatus();

	void HandleGameStatusChanged();
	UFUNCTION()
	void OnRep_CanJumpOffPlane();
	
	void HandleCanJumpOffPlaneChanged();
};
