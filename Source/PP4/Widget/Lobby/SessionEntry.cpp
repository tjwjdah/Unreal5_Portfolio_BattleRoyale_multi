// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntry.h"
#include "SessionsList.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void USessionEntry::NativeConstruct()
{
	Super::NativeConstruct();
	FString ServerName;
	const FOnlineSession& Sess = SessionResult.Session;
	const FOnlineSessionSettings& Settings = Sess.SessionSettings;
	if (!Settings.Get(SERVER_NAME_SETTINGS_KEY, ServerName)) {
		ServerName = TEXT("Unnamed");
	}
	int32 MaxPublic = Settings.NumPublicConnections;     
	int32 OpenPublic = Sess.NumOpenPublicConnections;
	ServerNameText->SetText(FText::FromString(ServerName));
	//PlayerCount->SetText(FText::Format(FText::FromString("{x}/{y}"), FText::AsNumber(MaxPublic - OpenPublic),FText::AsNumber(MaxPublic)));
	PingText->SetText(FText::Format(FText::FromString("{x} ms"), FText::AsNumber(SessionResult.PingInMs)));
	PingText->SetColorAndOpacity(SessionResult.PingInMs >= 150 ? FLinearColor::Red : FLinearColor::Green);
	DefaultBColor = MainBorder->GetBrushColor();
}

void USessionEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	MainBorder->SetBrushColor(FLinearColor(0.f,0.09523f,1.0f,0.701f));

}

void USessionEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (Select)
		ToggleSelect(Select);
	else
		MainBorder->SetBrushColor(DefaultBColor);
}

FReply USessionEntry::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	SessionList->OnSesisonEntrySelect(this);
	return FReply::Handled();
}

FReply USessionEntry::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	SessionList->OnEntryDoubleClick(this);
	return FReply::Handled();
}

FOnlineSessionSearchResult& USessionEntry::GetSessionResult()
{
	return SessionResult;
}

void USessionEntry::ToggleSelect(bool _Select)
{
	Select = _Select;
	MainBorder->SetBrushColor(Select ? FLinearColor(0.002f,0.29f,0.f,1.f) : DefaultBColor);
}

