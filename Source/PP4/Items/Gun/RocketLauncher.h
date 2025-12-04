// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../WeaponItem.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ARocketLauncher : public AWeaponItem
{
	GENERATED_BODY()
	
public:
		ARocketLauncher();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Rocket;

public:
	virtual void BeginPlay() override;
	
	virtual void Reload(int32 _AvailableAmmo, int32& _Used, int32& _RemainingAmmo) override;


	UFUNCTION(NetMulticast, Unreliable)
	void McSetShowHideRocket(bool _Show);

	virtual void MCPlaySoundandEffect_Implementation() override;
};
