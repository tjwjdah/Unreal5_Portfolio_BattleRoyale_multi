// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "OverviewMap.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UOverviewMap : public UUserWidget
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UMinimapComponent> MinimapComponent;

protected:
	virtual void NativeConstruct() override;


public:
	UMinimapComponent* GetMinimapComponent() { return MinimapComponent; }


};
