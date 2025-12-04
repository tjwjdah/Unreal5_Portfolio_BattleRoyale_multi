// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EngageTarget.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UEngageTarget : public UBTTaskNode
{
	GENERATED_BODY()

	public:
	UEngageTarget();
	~UEngageTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
