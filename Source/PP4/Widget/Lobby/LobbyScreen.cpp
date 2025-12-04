// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyScreen.h"
#include "SessionsList.h"
#include "PlayerEntry.h"

#include "../../Main/BRGameInstance.h"

#include "Components/CheckBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
void ULobbyScreen::NativeConstruct()
{
	Super::NativeConstruct();
	if (ExitGameBtn)
		ExitGameBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnExitGameBtnClicked);
	
	if (FindGameBtn)
		FindGameBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnFindGameBtnClicked);
	
	if (HostGameBtn)
		HostGameBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnHostGameBtnClicked);
	
	if (JoinServerBtn)
		JoinServerBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnJoinServerBtnClicked);
	
	if (NextBtn)
		NextBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnNextBtnClicked);
	
	if (PreviousBtn)
		PreviousBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnPreviousBtnClicked);
	
	if (CloseSettingsBtn)
		CloseSettingsBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnCloseSettingsBtnClicked);

	if (SetNameBtn)
		SetNameBtn->OnClicked.AddDynamic(this, &ULobbyScreen::OnSetNameBtnClicked);

	
	if (IsListen) {
		HostText->SetText(FText::FromString(TEXT("LOAD LEVEL")));
		HostSuccess();
	}
}

void ULobbyScreen::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
}


void ULobbyScreen::OnExitGameBtnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void ULobbyScreen::OnFindGameBtnClicked()
{
	if (!IsValid(SessionList)) 
		SessionList = Cast<USessionsList>(CreateWidget(GetWorld(), SessionsListClass));
	
	SessionList->AddToViewport();
}

void ULobbyScreen::OnHostGameBtnClicked()
{
	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (!Inst)return;

	if (HostText->GetText().ToString() == TEXT("LOAD LEVEL")) {
		Inst->LoadGame(PlayerC);
	}
	else {
		if (!HostSettingsBox->IsVisible())
			HostSettingsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		else
		{
			if (MaxPlayersText->GetText().ToString().IsNumeric() && FCString::Atoi(*MaxPlayersText->GetText().ToString()) > 0) {
				HostGameBtn->SetIsEnabled(false);
				Inst->HostGame(PlayerC, FCString::Atoi(*MaxPlayersText->GetText().ToString()), UseLan->IsChecked(), GetTheGameMode(), FCString::Atoi(*MaxPlayersText->GetText().ToString())
					, MinPlayersText->GetText().ToString().IsNumeric() ? FCString::Atoi(*MinPlayersText->GetText().ToString()) : FCString::Atoi(*MaxPlayersText->GetText().ToString())
					, Botscb->IsChecked(), Ffire->IsChecked(), GetMapNameCombobox(), LoadMapCb->IsChecked(), Deathmatch->IsChecked(),ServerNameText->GetText().ToString());
			}
		}
	}
}

void ULobbyScreen::OnJoinServerBtnClicked()
{
	FString Address = IpAdrText->GetText().ToString();;
	if (Address.EndsWith(":0"))
	{
		Address.RemoveFromEnd(TEXT(":0"));
		Address += TEXT(":7777");
	}
	GetOwningPlayer()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, [this]() {
			if (this && IsValid(InfoText))
				JoinFailed();
		}, 3.0, false);
}

void ULobbyScreen::OnNextBtnClicked()
{
	ChangeCharacter(true);
}

void ULobbyScreen::OnPreviousBtnClicked()
{
	ChangeCharacter(false);
}

void ULobbyScreen::OnSetNameBtnClicked()
{
	if (NameTextBox->GetText().IsEmpty())return;
	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (!Inst)return;
	Inst->SetCustomCharName(NameTextBox->GetText().ToString());
}

void ULobbyScreen::OnCloseSettingsBtnClicked()
{
	HostSettingsBox->SetVisibility(ESlateVisibility::Hidden);
}

void ULobbyScreen::HostFailed()
{
	HostGameBtn->SetIsEnabled(true);
	HostText->SetText(FText::FromString(TEXT("HOST GAME")));
	InfoText->SetText(FText::FromString(TEXT("HOSTING GAME FAILED PLEASE TRY AGAIN!!")));
	InfoText->SetColorAndOpacity(FLinearColor::Red);
	InfoText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, [this]() {
			InfoText->SetVisibility(ESlateVisibility::Hidden);
		},2.f, false);

}

void ULobbyScreen::JoinFailed()
{
	InfoText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InfoText->SetText(FText::FromString(TEXT("Failed to Join!")));
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, [this]() {
			if (this)
				InfoText->SetVisibility(ESlateVisibility::Hidden);
		}, 2.f, false);
}

void ULobbyScreen::HostSuccess()
{
	HostGameBtn->SetIsEnabled(true);
	HostText->SetText(FText::FromString(TEXT("LOAD LEVEL")));
	InfoText->SetText(FText::FromString(TEXT("HOST SUCCESS, YOU CAN LOAD LEVEL!")));
	InfoText->SetColorAndOpacity(FLinearColor::Green);
	InfoText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, [this]() {
			if(this)
			InfoText->SetVisibility(ESlateVisibility::Hidden);
		}, 1.0, false);
		
}

void ULobbyScreen::FindSessionFailed()
{
	if (!IsValid(SessionList)) {
		SessionList = Cast<USessionsList>(CreateWidget(GetWorld(), SessionsListClass));

		SessionList->AddToViewport();
	}
	TArray<FOnlineSessionSearchResult> Fail;
	SessionList->OnSessionSearchResponse(Fail);
}

void ULobbyScreen::FindSessionSuccess(TArray<FOnlineSessionSearchResult>& _Sessions)
{
	SessionList->OnSessionSearchResponse(_Sessions);
}

void ULobbyScreen::JoinSessionFailed()
{
	SessionList->OnJoinFailed();
}

void ULobbyScreen::JoinSessionSucess()
{
	SessionList->OnJoinSuccess();
}



void ULobbyScreen::AddPlayerEntry(int32 _ID, FText _PlayerName, FText _PlayerPing)
{
	UPlayerEntry* PlayerEntry = Cast<UPlayerEntry>(CreateWidget(GetWorld(), PlayerEntryClass));
	PlayerEntry->SetID(_ID);
	PlayerEntry->SetPlayerName(_PlayerName);
	PlayerEntry->SetPlayerPing(_PlayerPing);
	PlayerList->AddChild(PlayerEntry);
	ConnectText->SetText(FText::Format(FText::FromString("CONNECTED PLAYERS ({x})"), FText::AsNumber(PlayerList->GetChildrenCount() - 1)));
}

void ULobbyScreen::UpdateEntryPing(int32 _ID, int32 _Ping)
{
	for (int32 i = 0; i < PlayerList->GetChildrenCount() ; i++) {
		CV(UPlayerEntry, Entry, PlayerList->GetChildAt(i));
		if (!IsValid(Entry))
			continue;
		if (Entry->GetID() == _ID) {
			Entry->UpdatePing(FText::Format(FText::FromString("Ping({x})"), FText::AsNumber(_Ping)));
			break;
		}
	}
}

void ULobbyScreen::RemoveEntry(int32 _ID)
{
	for (int32 i = 0; i < PlayerList->GetChildrenCount(); i++) {
		CV(UPlayerEntry, Entry, PlayerList->GetChildAt(i));
		if (!IsValid(Entry)) {
			continue;
		}
		if (Entry->GetID() == _ID) {
			PlayerList->RemoveChildAt(i);
			Entry->RemoveFromParent();
			break;
		}
	}
}

void ULobbyScreen::SetCharacterName(FText _Text)
{
	NameTextBox->SetText(_Text);
}

int32 ULobbyScreen::GetTheGameMode()
{
	if (ModeSelect->GetSelectedOption() == TEXT("Solo")) {
		return 0;
	}
	else  if (ModeSelect->GetSelectedOption() == TEXT("Duo")) {
		return 1;
	}
	else
		return 2;
}

FString ULobbyScreen::GetMapNameCombobox()
{
	if (MapCombobox->GetSelectedOption() == TEXT("Big (Plane)")) {
		return TEXT("MapWithAirplane/PlaneMainMap");
	}
	else if (MapCombobox->GetSelectedOption() == TEXT("Small (Teleport)")) {
		return TEXT("MainMap");
	}
	else
		return MapCombobox->GetSelectedOption();
}

void ULobbyScreen::ChangeCharacter(bool _Next)
{
	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (!Inst)return; int32 NextClass;

	if (_Next)
		NextClass = Inst->GetCharacterID()+1;
	else
		NextClass = Inst->GetCharacterID()-1;
	Inst->ChangeLobbyClass(NextClass >= Inst->GetLobbyCharacters().Num() ? 0 : (NextClass < 0 ? Inst->GetLobbyCharacters().Num() - 1 : NextClass));
}
