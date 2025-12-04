// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/SpectatorPawn.h"
#include "BRSpectator.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API ABRSpectator : public ASpectatorPawn
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRCharacter> SpectatingPlayer;


public:
	void SetSpectatingPlayer(ABRCharacter* _SpectatingPlayer) { SpectatingPlayer = _SpectatingPlayer; }
};
