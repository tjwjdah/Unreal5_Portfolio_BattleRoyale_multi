// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone.h"

#include "../Main/BRPlayerController.h"
#include "../Main/BRGameInstance.h"
#include "../Main/BRGameState.h"
#include "../Character/AI/AICharacter.h"
#include "../Character/BRCharacter.h"

#include "../Widget/InGameWidget.h"
#include "../Widget/Minimap/OverviewMap.h"
#include "../Widget/Minimap/MinimapComponent.h"

#include "../Items/DamageTypes/ZoneDamage.h"

#include "Math/UnrealMathUtility.h"

#include "Kismet/KismetMathLibrary.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FirstDelay = 5.0;
	DoDoce = false;
	
	ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
	ZoneMesh->SetWorldScale3D(FVector(InitialScale, InitialScale, ZoneZScale));

}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();

	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	Inst->SetZone(this);


	if (HasAuthority()) {
		if (Inst->IsDeathMatchGame()) {
			Destroy();
			return;
		}
		CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
		State->SetZone(this);
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZone::ZoneInitialized, 1.f, false);
}

// Called every frame
void AZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Chrink)return;
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("AZone Tick")));
	SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), FinalPoint, DeltaTime, ShrinkInterpSpeed));
	
	DeltaLoc = 1.f - ((FinalPoint - GetActorLocation()).Size() / LocDiff.Size());

	ZoneMesh->SetWorldScale3D(FMath::Lerp(InitScale, GoalScale, DeltaLoc));
	if (ZoneMesh->GetComponentScale().X <= FinalShrinkValue) {
		Chrink = false;
		if (HasAuthority()) {
			MCZoneEnded();
		}
	}
	else {
		if (DeltaLoc == 1.f) {
			Chrink = false;
			if (HasAuthority()) {
				MCStopShrink(ZoneMesh->GetComponentScale(), GetActorLocation(), false);
				GetNextPoint();
			}
		}
	}
}

void AZone::StartZone()
{
	SetActorHiddenInGame(false);
	TotatlSteps = Steps.Num();
	CurrentStep = Steps.Num();
	if (CurrentStep <= 0)return;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZone::DamageTimer, 1.f, true);
	ZoneStart = true;
	FVector2D RandLoc = GetRandomLocation();
	FinalPoint = GetActorLocation() + FVector(RandLoc.X, RandLoc.Y, 0.f);
	LocDiff = FinalPoint - GetActorLocation();
	InitScale = ZoneMesh->GetComponentScale();
	StepScale = InitScale / TotatlSteps;
	GoalScale = GetGoalScale();
	MCZoneStarted(FinalPoint, GoalScale);
	FTimerHandle TimerHandle1;
	GetWorldTimerManager().SetTimer(TimerHandle1, [this]() {
		if (SetupStep()) {
			MCDoShrink(FinalPoint, GoalScale, ShrinkInterpSpeed);
			Chrink = true;
			MCZoneMoving();
			FirstSetupDone = true;
		}
		}, FirstDelay, false);
}

void AZone::ZoneStarted()
{
	if(IsValid(PlayerController) && IsValid(PlayerController->GetInGameWidget())) {
		PlayerController->GetInGameWidget()->ZoneStarting(std::trunc(FirstDelay));
		ZoneInitialized();

	}
}

void AZone::ZonePaused()
{
	if (IsValid(PlayerController) && IsValid(PlayerController->GetInGameWidget())) {
		PlayerController->GetInGameWidget()->ZonePaused(std::trunc(PauseTime));
	}
}

void AZone::ZoneEnded()
{
	if (IsValid(PlayerController) && IsValid(PlayerController->GetInGameWidget())) {
		PlayerController->GetInGameWidget()->ZoneEnded();
	}
}

void AZone::ZoneMoving()
{
	if (IsValid(PlayerController) && IsValid(PlayerController->GetInGameWidget())) {
		PlayerController->GetInGameWidget()->ZoneMoving();
	}
}

void AZone::ZoneInitialized()
{
	if (DoDoce)return;

	DoDoce = true;
	
	ifv(PlayerController) {
		ABRCharacter* Char = Cast<ABRCharacter>(PlayerController->GetPawn());
		ifv(Char) {
			if(Char->GetOverviewMap())
				if(Char->GetOverviewMap()->GetMinimapComponent())
					Char->GetOverviewMap()->GetMinimapComponent()->AddZone(this);
		}
	}
}

void AZone::ZoneSuspended()
{
	if (IsValid(PlayerController) && IsValid(PlayerController->GetInGameWidget())) {
		PlayerController->GetInGameWidget()->ZoneSuspended();
	}
}

float AZone::GetRadius()
{
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);
	return BoxExtent.X / 2.f;
}

FVector2D AZone::GetRandomLocation()
{
	
	return FVector2D((GetRadius() / 2.f) * GetRandomMultiplier(), (GetRadius() / 2.f) * GetRandomMultiplier());
}

float AZone::GetRandomMultiplier()
{
	int32 Result=0;
	while(Result==0)
		Result= UKismetMathLibrary::RandomIntegerInRange(-1, 1);

	return Result;
}

FVector AZone::GetGoalScale()
{
	CurrentStep = CurrentStep - 1;

	if (CurrentStep > 0) {
		FVector GoalS = (StepScale * CurrentStep);
		return FVector(GoalS.X, GoalS.Y, ZoneZScale);
	}
	else {
		return FVector(FinalShrinkValue, FinalShrinkValue,ZoneZScale);
	}
	
}

bool AZone::SetupStep()
{
	const int32 Index = Steps.Num() - CurrentStep - 1;
	if (Steps.IsValidIndex(Index)) {
		ShrinkInterpSpeed = Steps[Steps.Num() - CurrentStep - 1].ShrinkSpeed;
		DamagePerSec = Steps[Steps.Num() - CurrentStep - 1].DamagePerSec;
		PauseTime = Steps[Steps.Num() - CurrentStep - 1].PauseTime;
		return true;
	}
	else
	return false;
}

void AZone::HandlePlayersInside()
{
	TArray<AActor*> OverlappingActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::ComponentOverlapActors(ZoneMesh, GetActorTransform(), ObjectTypes, ACharacter::StaticClass(), ActorsToIgnore, OverlappingActors);
	
	for (int32 i = 0; i < OverlappingActors.Num(); i++) {
		if (!PlayersOutside.Contains(OverlappingActors[i]))continue;

		PlayersOutside.Remove(OverlappingActors[i]);
		CV(ABRCharacter, Char, OverlappingActors[i]);

		if (!IsValid(Char)) {
			CV(AAICharacter, AIChar, OverlappingActors[i]);
			if (AIChar) {
				AIChar->InZone();
			}
			continue;
		}

		if (!Char->GetEnablePostProcessing()) continue;
		Char->PlayerInZoneStatus(true);
		Char->CLIENTPlayerInZoneStatus(true);
	}
	for (int32 i = 0; i < PlayerInCircle.Num(); i++) {
		if (OverlappingActors.Contains(PlayerInCircle[i]))continue;
		if (!IsValid(PlayerInCircle[i]))continue;
		PlayersOutside.Add(PlayerInCircle[i]);
		CV(ABRCharacter, Char, PlayerInCircle[i]);

		if (!IsValid(Char)) {
			CV(AAICharacter, AIChar, PlayerInCircle[i]);
			if (AIChar) {
				AIChar->OutZone();
			}
			continue;
		}

		if (!Char->GetEnablePostProcessing()) continue;
		Char->PlayerInZoneStatus(false);
		Char->CLIENTPlayerInZoneStatus(false);
	}
	PlayerInCircle = OverlappingActors;
}

void AZone::DamageTimer()
{
	HandlePlayersInside();

	for (int32 i = 0; i < PlayersOutside.Num(); i++) {
		if (!IsValid(PlayersOutside[i]))continue;
		CV(ABRCharacter, Char, PlayersOutside[i]);
		if (IsValid(Char))
			Char->DamageTaken(DamagePerSec, SourceName, nullptr, false);
		else
			UGameplayStatics::ApplyDamage(PlayersOutside[i], DamagePerSec, nullptr, this, UZoneDamage::StaticClass());
	}
}

void AZone::GetNextPoint()
{
	GoalScale = GetGoalScale();
	FVector2D RandLoc = GetRandomLocation();
	FinalPoint = GetActorLocation() + FVector(RandLoc.X, RandLoc.Y, 0.f);
	LocDiff = FinalPoint - GetActorLocation();
	InitScale = ZoneMesh->GetComponentScale();
	if (!SetupStep()) return;

	MCZonePaused(FinalPoint, GoalScale);
	
	if (CurrentStep <1) {
		GoalScale.X = FinalShrinkValue;
		GoalScale.Y = FinalShrinkValue;
		GoalScale.Z = ZoneZScale;
	}
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AZone::DelayedNextPoint, PauseTime, false);
}

void AZone::DelayedNextPoint()
{
	MCDoShrink(FinalPoint, GoalScale, ShrinkInterpSpeed);
	Chrink = true;
	MCZoneMoving();
}

void AZone::AdminStopZone()
{
	if (HasAuthority()) {
		if (Chrink) {
			Chrink = false;
			MCStopShrink(ZoneMesh->GetComponentScale(), GetActorLocation(),true);
		}
	}
}

void AZone::AdminResumeZone()
{
	if (HasAuthority()) {
		if (ZoneStart && FirstSetupDone) {
			Chrink = true;
			MCZoneMoving();
		}
	}

}

void AZone::AdminToggleStart()
{
	if (Chrink)
		AdminStopZone();
	else
		AdminResumeZone();
}



void AZone::MCDoShrink_Implementation(FVector _FinalPoint, FVector _GoalScale, float _ShrinkInterpSpeed)
{
	if (HasAuthority())return;
	ShrinkInterpSpeed = _ShrinkInterpSpeed;
	FinalPoint = _FinalPoint;
	LocDiff = FinalPoint - GetActorLocation();
	GoalScale = _GoalScale;
	InitScale = ZoneMesh->GetComponentScale();
	Chrink = true;
}
void AZone::MCStopShrink_Implementation(FVector _NewScale, FVector _NewLocation, bool _AdminForced)
{
	if (!HasAuthority()) {
		Chrink = false;
		ZoneMesh->SetWorldScale3D(_NewScale);
		SetActorLocation(_NewLocation);
	}
	if (_AdminForced)
		ZoneSuspended();
}
void AZone::MCZoneStarted_Implementation(FVector _NextPoint, FVector _TargetScale)
{
	ZoneStarted();
	SetActorHiddenInGame(false);
	MinimapNextLocation = _NextPoint;
	MinimapTargetScale = _TargetScale;
	DrawNextPoint = true;

}

void AZone::MCZonePaused_Implementation(FVector _NextPoint, FVector _TargetScale)
{
	ZonePaused();
	MinimapNextLocation = _NextPoint;
	MinimapTargetScale = _TargetScale;
	DrawNextPoint = true;
}

void AZone::MCZoneEnded_Implementation()
{
	ZoneEnded();
	DrawNextPoint = false;
}

void AZone::MCZoneMoving_Implementation()
{
	ZoneMoving();
}
