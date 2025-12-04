// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "../Character/AI/AICharacter.h"
#include "BRGameMode.generated.h"
/**
 * 
 */
UCLASS()
class PP4_API ABRGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABRGameMode();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlaneDropGameModeComp> PlaneDropGameModeComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AAICharacter>> AIClassArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
	 meta = (AllowPrivateAccess = "true"))
	FGameModeSettings Settings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimerHandle GameCountDownHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ForceStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool HandleOnPostLogin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 LatestSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 LatestSpawnPoint2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerStart> m_StartingZonePlayerStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerStart> m_ArenaPlayerStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APlayerStart> m_AIPlayerStart;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FLinearColor> Colors;
	
	bool DoOnce = false;
	bool DoOnce2 = false;
	FTimerHandle RetriggerableDelayHandle;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void BeginPlay() override;

	virtual void Logout(AController* Exiting) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer) override;

	void GetSettings();

	void ForceStartGame();

	/**
	 * Called during RestartPlayer to actually spawn the player's pawn, when using a transform
	 * @param       NewPlayer - Controller for whom this pawn is spawned
	 * @param       SpawnTransform - Transform at which to spawn pawn
	 * @return      a pawn of the default pawn class
	 *
	 * NOTE: This function is linked to BlueprintNativeEvent: AGameModeBase::SpawnDefaultPawnAtTransform
	 */
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	/**
	 * Return the 'best' player start for this player to spawn from
	 * Default implementation looks for a random unoccupied spot
	 *
	 * @param Player is the controller for whom we are choosing a playerstart
	 * @returns AActor chosen as player start (usually a PlayerStart)
	 *
	 * NOTE: This function is linked to BlueprintNativeEvent: AGameModeBase::ChoosePlayerStart
	 */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;


	TSubclassOf<class ABRCharacter> GetCharacterRandomClass(int32 _CID);

	APlayerStart* GetRandomSpawnLocation();

	bool IsGameTime(bool _Forced);

	virtual void StartGame();

	void UpdateKillFeed(ACharacter* _Killed, ACharacter* _Killer, FString _Source);

	FText GetPlayerName(ACharacter* _Player);

	void HandleTeamWinCondition();

	void HandleSoloWinCondition();

	FTransform GetRandomSpawnLocation_Arena();

	FLinearColor GetGoodColor();

	bool IsDeathMatch();

	void SpawnAI(bool _CheckGameTime);

	void OnAIDeathCheckWinCondition();

	void SpawnAIClass(TSubclassOf<AAICharacter> _Class, bool _CheckGameTimer);

	
	void CheckFillBots();

	TSubclassOf<AAICharacter> GetRandomAIClass();

	void CheckIfGameTime();


	template <typename T>
	void StartRetriggerableDelay(void(T::* _Func)(),float _DelayTime)
	{
		GetWorldTimerManager().ClearTimer(RetriggerableDelayHandle);
		GetWorldTimerManager().SetTimer(RetriggerableDelayHandle, FTimerDelegate::CreateUObject(this, _Func), _DelayTime, false);
	}
	
	void GameCountDown();

	void StartGameSetTimer();

	void RequestSpawnWithID(int32 ClassID, APlayerController* _Controller);
	
	void SpawnOnDeathMatch(APlayerController* _Controller, TSubclassOf<ABRCharacter> _Character,int32 TeamID);

	void RequestSpawnAI(bool _CheckGameTime);

	void PlayerDied(ABRCharacter* _Killed, ACharacter* _Killer, FString _DamageSource);

	void GameEnded();
	
	UPlaneDropGameModeComp* GetPlaneDropGameModeComp() { return PlaneDropGameModeComp; };


};
/*
클래스				타입				복제 / RPC 지원								어디 존재
GameMode			 A					RPC / Rep 자체는 가능하나 클라에 없음		 서버만
GameState			 A					?(서버→클라)								 서버 + 클라
PlayerState			 A					?(서버→클라)								 서버 + 클라
PlayerController	 A					?(소유자 기반)							 서버 + 해당 클라
Pawn / Character	 A					?										 서버 + 클라
GameInstance		 U					?	각 프로세스							 로컬
ActorComponent		 U(특수)		?(소유 Actor가 Replicate될 때)	서버 + 클라(소유 Actor 따름)
*/