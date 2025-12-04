// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "TeammateIcon.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UTeammateIcon : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> PositionIcon;
	
protected:
	virtual void NativeConstruct();
public:
	void Initialize(FLinearColor _Color);
};
