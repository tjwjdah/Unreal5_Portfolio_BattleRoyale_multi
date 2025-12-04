// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneDropGameModeComp.h"
#include "PlaneAirDrop.h"
#include "../Main/BRGameState.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlaneDropGameModeComp::UPlaneDropGameModeComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AActor>	DropAirPlaneSpawn(TEXT("/Game/Maps/Plane/BPPlaneAirDrop"));
	if (DropAirPlaneSpawn.Succeeded())
		DropAirPlaneSpawnClass = DropAirPlaneSpawn.Class;

	static ConstructorHelpers::FClassFinder<AActor>	LootBoxSpawn(TEXT("/Game/Maps/Plane/BPAirDropBox"));
	if (LootBoxSpawn.Succeeded())
		LootBoxSpawnClass = LootBoxSpawn.Class;
	

	static ConstructorHelpers::FClassFinder<AActor>	SpawnPoint(TEXT("/Game/Maps/Plane/BPSpawnPoint"));
	if (SpawnPoint.Succeeded())
		SpawnPointsClass = SpawnPoint.Class;

	static ConstructorHelpers::FClassFinder<AActor>	PlaneDestination(TEXT("/Game/Maps/Plane/BPPlaneDestination"));
	if (PlaneDestination.Succeeded())
		PlaneDestinationsClass = PlaneDestination.Class;

	static ConstructorHelpers::FClassFinder<AActor>	BoxSpawnsLoc(TEXT("/Game/Maps/Plane/BPBoxSpawnsLoc"));
	if (BoxSpawnsLoc.Succeeded())
		BoxSpawnsLocsClass = BoxSpawnsLoc.Class;
	MaxNumPlaneSpawns = 3;
	IntervalBetweenSpawns = 30.f;
	PlaneMaxSpeed = 20000.f;
	PlaneAcceleration = 15000.f;
	

	// ...
}


// Called when the game starts
void UPlaneDropGameModeComp::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	SetupDelays();
	if (!GetOwner()->HasAuthority())return;
	NumPlaneSpawnedCounter = 0;
	Init();
}


// Called every frame
void UPlaneDropGameModeComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UPlaneDropGameModeComp::Init()
{
	CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
	if (!State)return;
	if (!InstantSpawn) {
		if (State->GetGameStatus() != EGameStatus::Started) {
			FTimerHandle TimerHandle2;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &UPlaneDropGameModeComp::Init, 10.f, false);
			return;
		}
	}
	TArray<AActor*> Found;
	

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
	UGameplayStatics::GetAllActorsOfClass(World, SpawnPointsClass, SpawnPoints);
	UGameplayStatics::GetAllActorsOfClass(World, PlaneDestinationsClass, PlaneDestinations);
	UGameplayStatics::GetAllActorsOfClass(World, BoxSpawnsLocsClass, BoxSpawnsLocs);
	/*
	if (SpawnPoints.Num() == 0) {
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("No SpawnPoints")));
	}
	if (PlaneDestinations.Num() == 0) {
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("No PlaneDestinations")));
	}
	else {
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("No PlaneDestinations")));
	}
	if (BoxSpawnsLocs.Num() == 0) {
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("No BoxSpawnsLocs")));
	}
	*/
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneDropGameModeComp::HandleSpawn, 10, true);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneDropGameModeComp::HandleSpawn, IntervalBetweenSpawns, true,FirstSpawnDelay);
}

void UPlaneDropGameModeComp::SetupDelays()
{
	IntervalBetweenSpawns = FMath::Abs(IntervalBetweenSpawns);
	FirstSpawnDelay = FMath::Abs(FirstSpawnDelay);
	/*
	if (FirstSpawnDelay <= IntervalBetweenSpawns) {
		FirstSpawnDelay = (IntervalBetweenSpawns - FirstSpawnDelay) * -1.f;
		if (FirstSpawnDelay == 0.f)
			FirstSpawnDelay = IntervalBetweenSpawns * -1.f;
	}
	else {
		FirstSpawnDelay = FirstSpawnDelay - IntervalBetweenSpawns;
	}
	*/
}

void UPlaneDropGameModeComp::HandleSpawn()
{
	
	FActorSpawnParameters	param;
	FTransform Trans;
	Trans.SetLocation(SpawnPoints[UKismetMathLibrary::RandomInteger(SpawnPoints.Num())]->GetActorLocation());
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	APlaneAirDrop* AirDrop= GetWorld()->SpawnActor<APlaneAirDrop>(DropAirPlaneSpawnClass,Trans,param);
	AirDrop->SetSpeed(PlaneMaxSpeed, PlaneAcceleration, PlaneAcceleration);
	AirDrop->SetLootBoxSpawnSettings(LootBoxSpawnClass, BoxSpawnsLocs[UKismetMathLibrary::RandomInteger(BoxSpawnsLocs.Num())]->GetActorLocation());
	AirDrop->SetDestination(PlaneDestinations[UKismetMathLibrary::RandomInteger(PlaneDestinations.Num())]->GetActorLocation());
	NumPlaneSpawnedCounter++;
	if (NumPlaneSpawnedCounter >= MaxNumPlaneSpawns) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		TimerHandle.Invalidate();
	}
}

