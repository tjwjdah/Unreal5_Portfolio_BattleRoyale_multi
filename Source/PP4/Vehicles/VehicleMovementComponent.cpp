// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMovementComponent.h"

#include "Vehicle.h"
#include "ChaosWheeledVehicleMovementComponent.h"

// Sets default values for this component's properties
UVehicleMovementComponent::UVehicleMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
   
	// ...
}


// Called when the game starts
void UVehicleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
  
}

FVehicleMove UVehicleMovementComponent::CreateMove(float Delta)
{
    FVehicleMove Move;
    Move.DeltaTime = Delta;
    Move.Time = GetWorld()->TimeSeconds;
    Move.Throttle = FMath::Clamp(Throttle, -1.f, 1.f);
    Move.Steering = FMath::Clamp(Steering, -1.f, 1.f);
    Move.Brake = FMath::Clamp(Brake, 0.f, 1.f);
    Move.Handbrake = bHandbrake;

    return Move;
}


// Called every frame
void UVehicleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    if (!IsValid(Owner))
        Owner = Cast<AVehicle>(GetOwner());
    if (!IsValid(Owner)) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

    if (Owner->IsLocallyControlled())
    {
        LastMove = CreateMove(DeltaTime);
        SimulateMove(LastMove);
    }
}

void UVehicleMovementComponent::SimulateMove(FVehicleMove _Move)
{
    if (!IsValid(Owner))return;

    auto* M = Cast<UChaosWheeledVehicleMovementComponent>(Owner->GetVehicleMovement());
    if (!M) return;

    if (_Move.Throttle >= 0.f)
    {

        M->SetBrakeInput(0.f);
        M->SetThrottleInput(FMath::Clamp(_Move.Throttle, 0.f, 1.f));
    }
    else
    {
        const float a = FMath::Clamp(-_Move.Throttle, 0.f, 1.f);
        M->SetBrakeInput(a);
        M->SetThrottleInput(0.f);
    }
    M->SetSteeringInput(_Move.Steering);
    
}

void UVehicleMovementComponent::SetVelocity(FVector Val)
{
    if (USkeletalMeshComponent* Mesh = Owner->GetMesh())
    {
        Mesh->SetPhysicsLinearVelocity(Val, false, NAME_None);
    }
    return;
   // { Velocity = Val; }
}

