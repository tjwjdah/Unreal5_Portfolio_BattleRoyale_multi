// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneAirDrop.h"
#include "AirDropBox.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlaneAirDrop::APlaneAirDrop()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Arrow);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(StaticMesh);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	DoOnce = false;
}

// Called when the game starts or when spawned
void APlaneAirDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaneAirDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!Go || !HasAuthority())return;

	FloatingPawnMovement->AddInputVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()));

	if (GoFinalDes) {
		if (!MissionDone) {
		
			TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Destination);
			CurrentRot = FMath::RInterpTo(GetActorRotation(), FRotator(TargetRot.Pitch,TargetRot.Yaw, GetLean(TargetRot)), DeltaTime, 1.f);
			SetActorRotation(CurrentRot, ETeleportType::TeleportPhysics);
			DistanceDestination = (FVector(Destination.X, Destination.Y, 0) - FVector(GetActorLocation().X, GetActorLocation().Y, 0)).Size();
			if (DistanceDestination < PlaneEndDistination) {
				if (DoOnce)return;
				DoOnce = true;
				MissionDone = true;
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this] {
					Destroy();
					}, 10.f, false);
			}
		}
	}
	else {
		DistanceBox = abs((FVector(BoxLocation.X, BoxLocation.Y, 0.f) - FVector(GetActorLocation().X, GetActorLocation().Y, 0.f)).Size());
		if (DistanceBox < 1000.f) {
			GoFinalDes = true;
			FActorSpawnParameters	param;
			FTransform Trans;
			Trans.SetLocation(GetActorLocation()+FVector(0.f,0.f,-1500.f)+FVector(UKismetMathLibrary::RandomFloatInRange(-500.f,500.f), UKismetMathLibrary::RandomFloatInRange(-500.f, 500.f), 0.f));
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			AAirDropBox* AirDrop = GetWorld()->SpawnActor<AAirDropBox>(BoxSpawn, Trans, param);
		}
	}
}

// Called to bind functionality to input
void APlaneAirDrop::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlaneAirDrop::SetSpeed(float _MaxSpeed, float _Acceleration, float _Deceleration)
{
	FloatingPawnMovement->MaxSpeed = _MaxSpeed;
	FloatingPawnMovement->Acceleration = _Acceleration;
	FloatingPawnMovement->Deceleration = _Deceleration;
}

void APlaneAirDrop::SetLootBoxSpawnSettings(TSubclassOf<AAirDropBox> _Box, FVector _Loc)
{
	BoxSpawn = _Box;
	BoxLocation = _Loc;
}

void APlaneAirDrop::SetDestination(FVector _Vector)
{
	Destination = _Vector;
	if (HasAuthority()) {
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BoxLocation), ETeleportType::TeleportPhysics);
		Go = true;
	}
}

float APlaneAirDrop::GetLean(FRotator _Rot)
{
	if (abs(UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), _Rot).Yaw) > 15.f) {
		return UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), _Rot).Yaw * -1.f;
	}
	return 0.0f;
}

