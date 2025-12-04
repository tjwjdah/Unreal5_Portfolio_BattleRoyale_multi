// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */

UCLASS()
class PP4_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UMinimapComponent> MinimapComponent;
	


public:
	UMinimapComponent* GetMinimapComponent() { return MinimapComponent; }
};
