// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerState.h"
#include "BRPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABRPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	int32 kills;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	FString RepCustomName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

public:
	const FString& GetRepCustomName() { return RepCustomName; }
	void SetRepCustomName(const FString& _CustomName) { RepCustomName = _CustomName; }
};
