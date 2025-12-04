// Fill out your copyright notice in the Description page of Project Settings.


#include "EngageTarget.h"
#include "AIController.h"

#include "../AICharacter.h"

UEngageTarget::UEngageTarget()
{
	NodeName = TEXT("EngageTarget");
	bNotifyTick = false; 
}

UEngageTarget::~UEngageTarget()
{
}

EBTNodeResult::Type UEngageTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return EBTNodeResult::Succeeded;
	AIController->StopMovement();
	APawn* Pawn = AIController->GetPawn();
	if (!IsValid(Pawn)) return EBTNodeResult::Succeeded;

	CV(AAICharacter, Char, Pawn);
	if (!IsValid(Char)) return EBTNodeResult::Succeeded;
	Char->FireWeapon();
	return EBTNodeResult::Succeeded;
}
