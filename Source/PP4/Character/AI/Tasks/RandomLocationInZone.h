// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomLocationInZone.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API URandomLocationInZone : public UBTTaskNode
{
	GENERATED_BODY()

	public:
		URandomLocationInZone();
	~URandomLocationInZone();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector	Key;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector	Zone;
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
