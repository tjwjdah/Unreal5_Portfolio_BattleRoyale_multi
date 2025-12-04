// Fill out your copyright notice in the Description page of Project Settings.


#include "AirPlane.h"

#include "../Main/BRPlayerController.h"
#include "../Main/PlaneGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

// Sets default values
AAirPlane::AAirPlane()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Arrow);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(StaticMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(StaticMesh);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	DistanceToDestinationToAllowJump = 280000.f;
	ForcedJumpDistance = 2000.f;
	DoOnce = true;
	DoOnce1 = true;
}

// Called when the game starts or when spawned
void AAirPlane::BeginPlay()
{
	Super::BeginPlay();
	
	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController())return;
	CV(ABRPlayerController, Cont, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(Cont))return;
	Cont->PlaneSpawned(this);
}

// Called every frame
void AAirPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!AirPlaneGo || !HasAuthority())return;

	FloatingPawnMovement->AddInputVector(UKismetMathLibrary::GetForwardVector(GetActorRotation()));

	FVector Actor2D = FVector(GetActorLocation().X, GetActorLocation().Y, 0.0f);
	FVector Destination2D = FVector(Destination.X, Destination.Y, 0.0f);
	DistanceToDestination = abs(UKismetMathLibrary::VSize(Destination2D - Actor2D));
	//비행기와 최종 목적지 사이의 거리 차이는 지도 크기에 따라 변경.
	if (DistanceToDestination <= DistanceToDestinationToAllowJump) {
		if (!DoOnce)return;
		DoOnce = false;
		CV(APlaneGameMode,GameMode,UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(GameMode))
			GameMode->CanJumpOffPlane();
	}
	if (DistanceToDestination < ForcedJumpDistance) {
		if (!DoOnce1)return;
		DoOnce1 = false;
		CV(APlaneGameMode, GameMode, UGameplayStatics::GetGameMode(GetWorld()));
		if (IsValid(GameMode)) {
			GameMode->FinalCheckpointReached();
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AAirPlane::DestroyDelay, 10.f, false);
		}
	}
}

// Called to bind functionality to input
void AAirPlane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAirPlane::SetDestination(FVector _Destination)
{
	Destination = _Destination;
	if (HasAuthority()) {
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Destination));
		AirPlaneGo = true;
	}
}

void AAirPlane::SetViewRotation(FRotator _Rotation)
{
	SpringArm->AddLocalRotation(FRotator(_Rotation.Pitch, _Rotation.Yaw, 0.f));
	SpringArm->SetRelativeRotation(FRotator(FMath::Clamp(SpringArm->GetRelativeRotation().Pitch, -80.f, 80.f), SpringArm->GetRelativeRotation().Yaw, 0.f));
}

void AAirPlane::SetSpeed(float _MaxSpeed, float _Acceleration, float _Deceleration)
{
	FloatingPawnMovement->MaxSpeed = _MaxSpeed;
	FloatingPawnMovement->Acceleration = _Acceleration;
	FloatingPawnMovement->Deceleration = _Deceleration;
}

void AAirPlane::SetDistanceToAllowJump(float _Dintance)
{
	DistanceToDestinationToAllowJump = _Dintance;

}

