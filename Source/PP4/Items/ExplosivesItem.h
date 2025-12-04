// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "PickupItem.h"
#include "ExplosivesItem.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AExplosivesItem : public APickupItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	FName AttachSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ExplosiveProjectile;

public:
	virtual void BeginPlay() override;

	void SetAttachSocketName(FName _Name) { AttachSocketName = _Name; }


	void SpawnExplosive(FVector _Location, FRotator _Rotator,AActor* _Actor);
};
