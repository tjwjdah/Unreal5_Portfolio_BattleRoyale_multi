// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "SessionsList.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API USessionsList : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USessionEntry> SelectedEntryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USessionEntry> CurrentSelectedEntry;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool AttemptJoining;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UCheckBox> CheckBox;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UCircularThrobber> CircularThrobber;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UComboBoxString> ComboBoxString;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UButton> FindSessionsBtn;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UTextBlock> InfoText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<class UVerticalBox> SessionsListVB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	TObjectPtr<UButton> CloseBtn;



protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnFindSessionsClicked();
	UFUNCTION()
	void OnCloseClicked();


	void OnSessionSearchResponse(TArray<FOnlineSessionSearchResult>& _Sessions);
	void OnSesisonEntrySelect(USessionEntry* _Entry);
	void OnEntryDoubleClick(USessionEntry* _Entry);
	void OnJoinSuccess();
	void OnJoinFailed();
};
