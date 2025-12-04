// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTService.h"
#include "Detect.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UDetect : public UBTService
{
	GENERATED_BODY()
	
	public:
	UDetect();


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


public:
	bool CheckDistance(AActor* _Actor1, AActor* _Actor2, float _Distance);

	bool SearchTarget(class ABRAIController* _AIController, APawn* _TargetPawn);
};
