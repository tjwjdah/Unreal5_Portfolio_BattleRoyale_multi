// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Engine/SceneCapture2D.h"
#include "MapCapture2D.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AMapCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

public:
	AMapCapture2D();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


};
