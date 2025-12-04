// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeMenu.h"

#include "../Main/BRPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
void UEscapeMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (CencelBtn)
		CencelBtn->OnClicked.AddDynamic(this, &UEscapeMenu::OnCancelClicked);

	if (ExitLobyBtn)
		ExitLobyBtn->OnClicked.AddDynamic(this, &UEscapeMenu::OnExitLobyClicked);


	if (QuitGameBtn)
		QuitGameBtn->OnClicked.AddDynamic(this, &UEscapeMenu::OnQuitGameClicked);
}

void UEscapeMenu::OnCancelClicked()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	RemoveFromParent();
}

void UEscapeMenu::OnExitLobyClicked()
{
	
	CV(ABRPlayerController, Cont, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Cont)
		Cont->GoLobby();
}

void UEscapeMenu::OnQuitGameClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
