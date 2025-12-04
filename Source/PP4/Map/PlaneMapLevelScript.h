// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Engine/LevelScriptActor.h"
#include "PlaneMapLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API APlaneMapLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void MinimapSetting();

};
