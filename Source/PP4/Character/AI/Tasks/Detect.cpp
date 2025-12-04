// Fill out your copyright notice in the Description page of Project Settings.


#include "Detect.h"

#include "../BRAIController.h"
#include "../AICharacter.h"

#include "../../BRCharacter.h"

#include "../../../Main/BRGameState.h"
#include "../../../Main/BRGameInstance.h"
#include "../../../Main/BRGameMode.h"

#include "../../../Vehicles/Vehicle.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UDetect::UDetect()
{
	NodeName = TEXT("Detect");
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
	Interval = 0.25f;
}

void UDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CV(ABRAIController, Cont, OwnerComp.GetAIOwner());

	if (!Cont) return;
	CV(AAICharacter, Char, Cont->GetPawn());

	if (!IsValid(Cont->GetPawn()) || !IsValid(Char))
	{
		Cont->SetAttacking(false);
		return;
	}

	if (!Char->GetIsAllInZone())
	{
		Cont->SetAttacking(false);
		Cont->ClearFocus(EAIFocusPriority::Default);
		return;
	}
	CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
	if (!State) return;

	for (int32 i = 0; i < State->GetPlayerList().Num(); i++) {
		auto* CurrentPawn = State->GetPlayerList()[i]->GetPawn();
		if (!CheckDistance(CurrentPawn, Cont->GetPawn(), 5000.f))continue;
		if (SearchTarget(Cont,CurrentPawn))
			return;
	}
	for (int32 i = 0; i < State->GetPlayerAIList().Num(); i++) {
		auto* CurrentPawn = State->GetPlayerAIList()[i]->GetPawn();
		if (!CheckDistance(CurrentPawn, Cont->GetPawn(), 5000.f))continue;
		if (SearchTarget(Cont,CurrentPawn))
			return;
	}

	if (Cont->GetAttacking()) {
		Cont->SetAttacking(false);
		if (UBlackboardComponent* BB = Cont->GetBlackboardComponent()) {
			BB->SetValueAsObject(TEXT("TargetSpotted"), nullptr);
		}
		Cont->ClearFocus(EAIFocusPriority::Default);
		Cont->SetCurrentTarget(nullptr);
	}
}

bool UDetect::CheckDistance(AActor* _Actor1, AActor* _Actor2, float _Distance)
{
	if (!IsValid(_Actor1) || !IsValid(_Actor2))
		return false;

	return (_Actor1->GetActorLocation() - _Actor2->GetActorLocation()).Size() <= _Distance;
}

bool UDetect::SearchTarget(ABRAIController* _AIController,APawn* _TargetPawn)
{
	CV(ACharacter, MyChar, _AIController->GetPawn());
	CV(ACharacter, TargetChar, _TargetPawn);
	if (!MyChar || !TargetChar) return false;
	FHitResult HitResult;
	FVector Start = MyChar->GetMesh()->GetSocketLocation(TEXT("head"));
	FVector End = TargetChar->GetActorLocation();
	float Radius = 25.0f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(_AIController);
	Params.AddIgnoredActor(MyChar);
	Params.bTraceComplex = false;


	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);
	/*EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
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
	*/
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

	if (abs(UKismetMathLibrary::FindLookAtRotation(MyChar->GetActorLocation(), HitResult.GetActor()->GetActorLocation()).Yaw
		- MyChar->GetActorRotation().Yaw) > _AIController->GetTargetMaxAngle()) return false;

	CV(ACharacter, HitChar, HitResult.GetActor());
	if (_AIController->GetCurrentTarget() == HitChar)return true;
	_AIController->SetCurrentTarget(HitChar);
	if (UBlackboardComponent* BB = _AIController->GetBlackboardComponent()) {
		BB->SetValueAsObject(TEXT("TargetSpotted"), HitChar);
	}
	_AIController->SetFocus(HitChar);
	_AIController->SetAttacking(true);
	return true;;
}

