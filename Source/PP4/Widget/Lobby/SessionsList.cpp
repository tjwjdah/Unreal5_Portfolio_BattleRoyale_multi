// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionsList.h"
#include "SessionEntry.h"

#include "../../Main/BRGameInstance.h"

#include "OnlineSessionSettings.h"
#include "Components/CircularThrobber.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void USessionsList::NativeConstruct()
{
	Super::NativeConstruct();
	if (FindSessionsBtn)
		FindSessionsBtn->OnClicked.AddDynamic(this, &USessionsList::OnFindSessionsClicked);
	if (FindSessionsBtn)
		CloseBtn->OnClicked.AddDynamic(this, &USessionsList::OnCloseClicked);
}

void USessionsList::OnFindSessionsClicked()
{
	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (!Inst)return;
	SessionsListVB->ClearChildren();
	CircularThrobber->SetVisibility(ESlateVisibility::Visible);
	FindSessionsBtn->SetIsEnabled(false);
	Inst->FindSession(UGameplayStatics::GetPlayerController(GetWorld(), 0), FCString::Atoi(*ComboBoxString->GetSelectedOption()),CheckBox->IsChecked());
	InfoText->SetText(FText::FromString(TEXT("Finding sessions..")));
}

void USessionsList::OnCloseClicked()
{
	RemoveFromParent();
}

void USessionsList::OnSessionSearchResponse(TArray<FOnlineSessionSearchResult>& _Sessions)
{
	CircularThrobber->SetVisibility(ESlateVisibility::Hidden);
	FindSessionsBtn->SetIsEnabled(true);
	if (_Sessions.Num() <= 0) {
		InfoText->SetText(FText::FromString(TEXT("Found no sessions")));
		return;
	}
	InfoText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	for (int32 i = 0; i < _Sessions.Num(); i++) {
		USessionEntry* Entry = Cast<USessionEntry>(CreateWidget(GetWorld(), SelectedEntryClass));
		Entry->SetSessionResult(_Sessions[i]);
		Entry->SetOwningPlayer(GetOwningPlayer());
		Entry->SetSessionList(this);
		int32 MaxPublic = _Sessions[i].Session.SessionSettings.NumPublicConnections;
		SessionsListVB->AddChildToVerticalBox(Entry)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	}
	InfoText->SetText(FText::FromString(TEXT("Doube click to join a session.")));
}

void USessionsList::OnSesisonEntrySelect(USessionEntry* _Entry)
{
	if (CurrentSelectedEntry != _Entry) {
		if (IsValid(CurrentSelectedEntry))
			CurrentSelectedEntry->ToggleSelect(false);
		CurrentSelectedEntry = _Entry;
		CurrentSelectedEntry->ToggleSelect(true);
	}
}

void USessionsList::OnEntryDoubleClick(USessionEntry* _Entry)
{

	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (!Inst ||AttemptJoining)return;
	AttemptJoining = true;
	CircularThrobber->SetVisibility(ESlateVisibility::Visible);
	FindSessionsBtn->SetIsEnabled(false);
	FString ServerName;
	if (_Entry->GetSessionResult().Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName)) {
		ServerName = TEXT("Unnamed");
	}
	InfoText->SetText(FText::Format(FText::FromString("Joining {x} .."),FText::FromString(ServerName)));
	
	Inst->Join(UGameplayStatics::GetPlayerController(GetWorld(), 0), _Entry->GetSessionResult());
}

void USessionsList::OnJoinSuccess()
{
	AttemptJoining = false;
	InfoText->SetText(FText::FromString(TEXT("Join success!")));
	RemoveFromParent();
}

void USessionsList::OnJoinFailed()
{
	InfoText->SetText(FText::FromString(TEXT("Failed to join session!")));
	CircularThrobber->SetVisibility(ESlateVisibility::Hidden);
	FindSessionsBtn->SetIsEnabled(true);
	AttemptJoining = false;

}
