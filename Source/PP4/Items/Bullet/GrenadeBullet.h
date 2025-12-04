// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../Bullet.h"
#include "GrenadeBullet.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AGrenadeBullet : public ABullet
{
	GENERATED_BODY()
	
public:
	AGrenadeBullet();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> GrenadeStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> EmitterImpact;

	bool DoOnce;
public:
	UFUNCTION()
	void OnGrenadeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
