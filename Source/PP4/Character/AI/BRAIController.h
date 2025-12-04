// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "AIController.h"
#include "BRAIController.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABRAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABRAIController();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool Attacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacter> CurrentTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TargetMaxAngle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector InitLoc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString AIName;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> AITree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardData> AIBlackboard;

	bool DoOnce;
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	
	void SetAttacking(bool _Attacking) { Attacking = _Attacking; }
	bool GetAttacking() {return Attacking; }

	bool CheckDistance(AActor* _Actor1, AActor* _Actor2, float _Distance);

	//void LookEnemies();

	float GetTargetMaxAngle() { return TargetMaxAngle; }
	ACharacter* GetCurrentTarget() { return CurrentTarget; }
	void SetCurrentTarget(ACharacter* _Char) { CurrentTarget=_Char; }
	//bool SearchTarget(APawn* _Pawn);
};
