// Fill out your copyright notice in the Description page of Project Settings.


#include "BRAIController.h"
#include "AICharacter.h"

#include "../../Main/BRGameState.h"
#include "../../Main/BRGameInstance.h"
#include "../../Main/BRGameMode.h"

#include "../../Vehicles/Vehicle.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


ABRAIController::ABRAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>	AITreeAsset(TEXT("/Game/Characters/AI/BehaviorTree/BRAITree"));

	if (AITreeAsset.Succeeded())
		AITree = AITreeAsset.Object;
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardAsset(TEXT("/Game/Characters/AI/BehaviorTree/BRAIBoard"));

	if (BlackboardAsset.Succeeded())
		AIBlackboard = BlackboardAsset.Object;

	TargetMaxAngle = 100.f;

	DoOnce = true;
	
}


void ABRAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
	if (State->GetGameStatus() != EGameStatus::Started)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this, InPawn] {
			OnPossess(InPawn);
			}, 1.f, false);
		return;
	}
	State->AddAIController(this);
	
	if (!RunBehaviorTree(AITree)) {
		LOG(TEXT("Monster BehaviorTree Error!!"));
	}
	CV(AAICharacter, Char, GetPawn());
	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!IsValid(Char) || !IsValid(BlackboardComp))return;
	BlackboardComp->SetValueAsFloat(TEXT("FireInterval"), Char->GetFireInterval());
	BlackboardComp->SetValueAsObject(TEXT("Zone"), Inst->GetZone());
	InitLoc = Char->GetActorLocation();
}

void ABRAIController::OnUnPossess()
{
	Super::OnUnPossess();
	if (!DoOnce)return;
	DoOnce = false;
	CV(UBRGameInstance, inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (!inst->IsDeathMatchGame()) 
		return;
	CV(ABRGameMode, Mode, UGameplayStatics::GetGameMode(GetWorld()));
	Mode->RequestSpawnAI(true);
}

bool ABRAIController::CheckDistance(AActor* _Actor1, AActor* _Actor2, float _Distance)
{
	if (!IsValid(_Actor1) || !IsValid(_Actor2))
		return false;

	return (_Actor1->GetActorLocation() - _Actor2->GetActorLocation()).Size() <= _Distance;

}

/*
void ABRAIController::LookEnemies()
{
	CV(AAICharacter, Char, GetPawn());
	if (!IsValid(GetPawn()) || !IsValid(Char))
	{
		GetWorldTimerManager().ClearTimer(TimerH);
		Attacking = false;
		return;
	}
	
	if (!Char->GetIsAllInZone())
	{
		ClearFocus(EAIFocusPriority::Default);
		return;
	}
	CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
	
	for (int32 i = 0; i < State->GetPlayerList().Num(); i++) {
		auto* CurrentPawn = State->GetPlayerList()[i]->GetPawn();
		if (!CheckDistance(CurrentPawn, GetPawn(), 5000.f))continue;
		if (SearchTarget(CurrentPawn))
			return;
	}
	for (int32 i = 0; i < State->GetPlayerAIList().Num(); i++) {
		auto* CurrentPawn = State->GetPlayerAIList()[i]->GetPawn();
		if (!CheckDistance(CurrentPawn, GetPawn(), 5000.f))continue;
		if (SearchTarget(CurrentPawn))
			return;
	}

	if (Attacking) {
		Attacking = false;
		UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
		if (!IsValid(BlackboardComp))return;
		BlackboardComp->SetValueAsObject(TEXT("TargetSpotted"), nullptr);
		ClearFocus(EAIFocusPriority::Default);
		CurrentTarget = nullptr;
	}
}

bool ABRAIController::SearchTarget(APawn* _Pawn)
{

	CV(ACharacter, MyChar, GetPawn());
	CV(ACharacter, CurrentChar, _Pawn);
	FHitResult HitResult;
	FVector Start = MyChar->GetMesh()->GetSocketLocation(TEXT("head"));
	FVector End = CurrentChar->GetActorLocation();
	float Radius = 25.0f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(MyChar);
	Params.bTraceComplex = false;


	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);
	EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration; 
	if (DrawDebugType != EDrawDebugTrace::None) {
		FColor TraceColor = FColor::Red;
		FColor HitColor = FColor::Green;
		float DrawTime = 0.2f;

		DrawDebugSphere(GetWorld(), Start, Radius, 12, TraceColor, false, DrawTime);
		DrawDebugSphere(GetWorld(), End, Radius, 12, TraceColor, false, DrawTime);

		if (bHit) {
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, HitColor, false, DrawTime);
		}
	}

	if (!HitResult.bBlockingHit)return false;
	
	CV(ABRCharacter, HitBRChar, HitResult.GetActor());
	CV(AAICharacter, HitAIChar, HitResult.GetActor());
	if (!HitBRChar) {
		CV(AVehicle, Vehicle, HitResult.GetActor());
		if (IsValid(Vehicle)) {
			for (int32 i = 1; i <= 4 && !IsValid(HitBRChar); i++)
				HitBRChar = Vehicle->GetSeatOwner(i);
		}
	}
	if (IsValid(HitBRChar) && !HitBRChar->IsAlive()) return false;
	if (IsValid(HitAIChar) && !HitAIChar->IsAlive())return false;
	if (abs(UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), HitResult.GetActor()->GetActorLocation()).Yaw
		- GetPawn()->GetActorRotation().Yaw) > TargetMaxAngle) return false;

	CV(ACharacter, HitChar, HitResult.GetActor());
	if (CurrentTarget == HitChar)return true;
	CurrentTarget = HitChar;
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!IsValid(BlackboardComp))return false;
	BlackboardComp->SetValueAsObject(TEXT("TargetSpotted"), CurrentTarget);
	SetFocus(CurrentTarget);
	Attacking = true;
	return true;;
}

*/