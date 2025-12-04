// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API URandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
	public:
	URandomLocation();
	~URandomLocation();
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector	Key;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector	Zone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float	Radius;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
	void GetLocation();
};
