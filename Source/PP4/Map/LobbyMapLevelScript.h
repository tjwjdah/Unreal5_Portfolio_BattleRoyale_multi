// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Engine/LevelScriptActor.h"
#include "LobbyMapLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ALobbyMapLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	TSubclassOf<APawn> GetRandomLobbyClass();
};
