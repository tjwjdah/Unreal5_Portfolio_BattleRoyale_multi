// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneGameMode.h"
#include "BRGameState.h"
#include "BRPlayerController.h"
#include "../Character/BRCharacter.h"
#include "../Map/AirPlane.h"
#include "../Map/PlaneDropGameModeComp.h"
#include "Kismet/KismetMathLibrary.h"
APlaneGameMode::APlaneGameMode()
{


	static ConstructorHelpers::FClassFinder<AActor>	PlaneSpawnClass(TEXT("/Game/Maps/Plane/BPPlaneSpawnPoint"));
	if (PlaneSpawnClass.Succeeded())
		PlaneSpawn = PlaneSpawnClass.Class;

	static ConstructorHelpers::FClassFinder<AActor>	FinalCheckpointClass(TEXT("/Game/Maps/Plane/BPFinalCheckpoint"));
	if (FinalCheckpointClass.Succeeded())
		FinalCheckpoint = FinalCheckpointClass.Class;

	static ConstructorHelpers::FClassFinder<AAirPlane>	PlanePawnClass(TEXT("/Game/Maps/Plane/BPAirPlane"));
	if (PlanePawnClass.Succeeded())
		PlaneClass = PlanePawnClass.Class;
	

	GetPlaneDropGameModeComp()->FirstSpawnDelay = 30.f;
	GetPlaneDropGameModeComp()->MaxNumPlaneSpawns = 3.f;
	GetPlaneDropGameModeComp()->InstantSpawn = false;
	
}
void APlaneGameMode::BeginPlay()
{
	Super::BeginPlay();


}

void APlaneGameMode::RequestJumpOff(APlayerController* _Controller)
{
	CV(ABRPlayerController, Controller, _Controller);
	if (Controller) {
		CV(ABRCharacter, Char, Controller->GetMyPawnRef());
		ifv(Char) {
			if (Char->GetPlayerInfo().Movement == EMovementState::InPlane) {
				Char->SetActorLocationAndRotation(Plane->GetStaticMesh()->GetSocketLocation(TEXT("Jump")), FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
				Char->SetActorHiddenInGame(false);
				Controller->Possess(Char);
				Char->SetJumpingOffPlane(true);
				Controller->JumpOffSuccess();
			}
		}
	}
}

void APlaneGameMode::BRPlaneStartGame()
{
	CV(ABRGameState, State, GameState);
	if (!State) return;
	State->SetGameStatus(EGameStatus::Started);
	auto& PlayerList = State->GetPlayerList();
	for (int32 i = 0; i < PlayerList.Num(); i++) {
		if (IsValid(PlayerList[i]) && IsValid(PlayerList[i]->GetPawn())) {
			ABRCharacter* Char = Cast<ABRCharacter>(PlayerList[i]->GetPawn());
			if(Char){
				Char->GameStarted(true);
				Char->SetActorHiddenInGame(true);
				PlayerList[i]->UnPossess();
			}
			else {
				CV(ABRPlayerController, Controller, PlayerList[i]);
				if(IsValid(Controller->GetBRPawn()) && Controller->GetBRPawn()->GetIsVehicleDriver()) {
					Controller->GetBRPawn()->MCPlayerExitedVehicle();
					Controller->Possess(Controller->GetBRPawn());
					i--;
				}
			}
		}
	}

	if (PlaneClass) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlaneSpawn, FoundActors);
		if (FoundActors.Num() > 0) {

			FActorSpawnParameters	param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			FTransform Trans(FRotator::ZeroRotator, FoundActors[UKismetMathLibrary::RandomInteger(FoundActors.Num())]->GetActorLocation(), FVector::One());
			Plane=Cast<AAirPlane>(GetWorld()->SpawnActor<APawn>(PlaneClass, Trans,param));
			if (!Plane) return;
				Plane->SetSpeed(20000.f, 20000.f, 8000.f);
				Plane->SetDistanceToAllowJump(280000.f);
				TArray<AActor*> FoundActors2;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), FinalCheckpoint, FoundActors2);
				if (FoundActors2.Num() > 0) 
					Plane->SetDestination(FoundActors2[UKismetMathLibrary::RandomInteger(FoundActors2.Num())]->GetActorLocation());
		}
	}	
}

void APlaneGameMode::CanJumpOffPlane()
{
	CV(ABRGameState, State, GameState);
	if (!State)return;
	State->SetCanJumpOffPlane(true);

}

void APlaneGameMode::FinalCheckpointReached()
{
	CV(ABRGameState, State, GameState);
	if (State) {
		auto& PlayerList = State->GetPlayerList();
		for (int32 i = 0; i < PlayerList.Num(); i++) {
			RequestJumpOff(PlayerList[i]);
		}
	}
}

void APlaneGameMode::StartGame()
{
	BRPlaneStartGame();
}

