// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "BRGameMode.h"
#include "PlaneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API APlaneGameMode : public ABRGameMode
{
	GENERATED_BODY()
	
public:
	APlaneGameMode();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAirPlane> PlaneClass;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAirPlane> Plane;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PlaneSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> FinalCheckpoint;
	
	

	
public:
	virtual void BeginPlay() override;
	
	void RequestJumpOff(APlayerController* _Controller);

	void BRPlaneStartGame();

	void CanJumpOffPlane();

	void FinalCheckpointReached();

	virtual void StartGame() override;

	
};
