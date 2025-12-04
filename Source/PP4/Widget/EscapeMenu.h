// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "EscapeMenu.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API UEscapeMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UButton> CencelBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> ExitLobyBtn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> QuitGameBtn;

protected:
	virtual void NativeConstruct() override;


public:
	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnExitLobyClicked();

	UFUNCTION()
	void OnQuitGameClicked();
};
