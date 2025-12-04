// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Zone.generated.h"

UCLASS()
class PP4_API AZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZone();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> ZoneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector Scale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector FinalPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Chrink;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector LocDiff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector InitScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float InitialScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ZoneZScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FirstDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ShrinkInterpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PauseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ZoneStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DamagePerSec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString SourceName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 TotatlSteps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CurrentStep;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FZoneStep> Steps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector StepScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector GoalScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DeltaLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> PlayersOutside;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FinalShrinkValue;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> PlayerInCircle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool FirstSetupDone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector MinimapNextLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool DrawNextPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor ZoneCircleColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor ZoneNextPointColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector MinimapTargetScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRPlayerController> PlayerController;

	bool DoDoce;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartZone();

	void SetPlayerController(ABRPlayerController* _PlayerController) { PlayerController = _PlayerController; }

	void ZoneStarted();
	void ZonePaused();
	void ZoneEnded();
	void ZoneMoving();
	void ZoneInitialized();
	void ZoneSuspended();
	
	float GetRadius();

	FVector2D GetRandomLocation();
	
	float GetRandomMultiplier();

	FVector GetGoalScale();
	
	bool SetupStep();

	void HandlePlayersInside();

	void DamageTimer();

	void GetNextPoint();

	void DelayedNextPoint();

	void AdminStopZone();

	void AdminResumeZone();

	void AdminToggleStart();

	UStaticMeshComponent* GetZoneMesh() { return ZoneMesh; }

	FLinearColor GetZoneCircleColor() { return ZoneCircleColor; }

	FLinearColor GetZoneNextPointColor() { return ZoneNextPointColor; }

	bool GetDrawNextPoint() { return DrawNextPoint; }

	FVector GetMinimapTargetScale() { return MinimapTargetScale; }

	FVector GetMinimapNextLocation() { return MinimapNextLocation; }
	public:
		UFUNCTION(NetMulticast, Reliable)
		void MCDoShrink(FVector _FinalPoint, FVector _GoalScale, float _ShrinkInterpSpeed);

		UFUNCTION(NetMulticast, Reliable)
		void MCStopShrink(FVector _NewScale, FVector _NewLocation, bool _AdminForced);

		UFUNCTION(NetMulticast, Reliable)
		void MCZoneStarted(FVector _NextPoint, FVector _TargetScale);

		UFUNCTION(NetMulticast, Reliable)
		void MCZonePaused(FVector _NextPoint, FVector _TargetScale);

		UFUNCTION(NetMulticast, Reliable)
		void MCZoneEnded();

		UFUNCTION(NetMulticast, Reliable)
		void MCZoneMoving();



};
