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
    if (!IsValid(Owner)) return;

    auto* M = Cast<UChaosWheeledVehicleMovementComponent>(Owner->GetVehicleMovement());
    if (!M) return;

    float ThrottleInput = FMath::Clamp(_Move.Throttle, -1.f, 1.f);
    float BrakeInput = FMath::Clamp(_Move.Brake, 0.f, 1.f);

    M->SetSteeringInput(FMath::Clamp(_Move.Steering, -1.f, 1.f));
    M->SetHandbrakeInput(_Move.Handbrake);

    if (BrakeInput > 0.f || _Move.Handbrake) {
        M->SetThrottleInput(0.f);
        M->SetBrakeInput(BrakeInput);
    }
    else if (ThrottleInput >= 0.f) {
        M->SetThrottleInput(ThrottleInput);
        M->SetBrakeInput(0.f);
    }
    else {
        M->SetThrottleInput(0.f);
        M->SetBrakeInput(-ThrottleInput);
    }
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

