// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "VehicleMovementComponent.h"
#include "VehicleMovementReplicator.generated.h"

USTRUCT()
struct FVCState
{
	
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FTransform Tranform;

	UPROPERTY()
	FVector Velocity;
	
	UPROPERTY()
	FVehicleMove LastMove;
};
struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(float LerpRatio) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
	FVector InterpolateDerivative(float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PP4_API UVehicleMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FVCState ServerState;

	UPROPERTY()
	TArray<FVehicleMove> UnacknowledgedMoves;

	UPROPERTY()
	TObjectPtr<class AVehicle> Owner;

	UPROPERTY()
	TObjectPtr<UVehicleMovementComponent> MovementComponent;

	UPROPERTY() 
	TObjectPtr<USceneComponent> MeshOffsetRoot;
	
	float ClientTimeSinceUpdate = 0.f, ClientTimeBetweenLastUpdates = 0.f;
	FTransform ClientStartTransform; FVector ClientStartVelocity;

	float ClientSimulatedTime=0.f;

	float SendAccumulator = 0.f;
	static constexpr float kSendInterval = 1.f / 30.f;
public:	
	// Sets default values for this component's properties
	UVehicleMovementReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	
	void ClearAcknowledgeMoves(FVehicleMove LastMove);

	void UpdateServerState(const FVehicleMove& Move);

	void ClientTick(float DeltaTime);
	FHermiteCubicSpline CreateSpline();
	void InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio);
	void InterpolateRotation(float LerpRatio);
	float VelocityToDerivative();
	
	UFUNCTION()
	void OnRep_ServerState();

	void Local_OnRep();

	void Simul_OnRep();

	//UFUNCTION(Server, Reliable, WithValidation)
	UFUNCTION(Server, Reliable)
	void Server_SendMove(FVehicleMove _Move);
};
