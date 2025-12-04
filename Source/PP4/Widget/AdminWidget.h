// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "AdminWidget.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UAdminWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
	protected:
	virtual void NativeConstruct() override;


};
