// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "VehicleMovementComponent.generated.h"

USTRUCT()
struct FVehicleMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle=0.f;
	UPROPERTY()
	float Steering =0.f;
	
	UPROPERTY() 
	float Brake = 0.f;
	UPROPERTY() 
	bool  Handbrake = false;

	UPROPERTY()
	float DeltaTime = 0.f;
	UPROPERTY()
	float Time = 0.f;
	bool IsValid() const
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(Steering) <= 1 &&
			Brake >= 0.f && Brake <= 1.f;
	}
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PP4_API UVehicleMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<class AVehicle> Owner;

	float Throttle = 0, Steering = 0, Brake = 0; bool bHandbrake = false;
	FVehicleMove LastMove;

	FVector Velocity;

public:	
	// Sets default values for this component's properties
	UVehicleMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FVehicleMove CreateMove(float Delta);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetThrottle(float Val) { Throttle = Val; };
	void SetSteeringThrow(float Val) { Steering = Val; };
	void SetBrake(float V) { Brake = V; }
	void SetHandbrake(bool b) { bHandbrake = b; }
	void SimulateMove(FVehicleMove _Move);
	void SetVelocity(FVector Val);
	FVector GetVelocity() { return Velocity; }
	FVehicleMove GetLastMove() { return LastMove; }

	
};
