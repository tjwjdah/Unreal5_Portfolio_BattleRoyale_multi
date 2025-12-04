// Fill out your copyright notice in the Description page of Project Settings.


#include "OnGameEndedWidget.h"

#include "../Main/BRPlayerController.h"
#include "../Main/BRGameInstance.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/BackgroundBlur.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UOnGameEndedWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ExitBtn)
		ExitBtn->OnClicked.AddDynamic(this, &UOnGameEndedWidget::OnExitBtnClicked);

	if (LobyBtn)
		LobyBtn->OnClicked.AddDynamic(this, &UOnGameEndedWidget::OnLobyBtnClicked);

	if (SpectateTeammateBtn)
		SpectateTeammateBtn->OnClicked.AddDynamic(this, &UOnGameEndedWidget::OnSpectateTeammateBtnClicked);

	if (SpectateRandomBtn)
		SpectateRandomBtn->OnClicked.AddDynamic(this, &UOnGameEndedWidget::OnSpectateRandomBtnClicked);

	FText FormattedText = FText::Format(FText::FromString(TEXT("# {0}")),FText::AsNumber(Rank));
	RankText->SetText(FormattedText);

	if (Rank == 1) 
		MessageText->SetText(FText::FromString(TEXT("WELL PLAYED!!")));
	else
		MessageText->SetText(FText::FromString(TEXT("BETTER LUCK NEXT TIME!!")));
}
void UOnGameEndedWidget::SetUpSpectating(APawn* _SpectatingPlayer)
{
	SpectatingPlayer = _SpectatingPlayer;
	BackgroundBlur->SetBlurStrength(0.f);
	HorizontalBox->SetVisibility(ESlateVisibility::Hidden);
	MessageText->SetVisibility(ESlateVisibility::Hidden);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, [this]() {
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(SpectatingPlayer,0.1,EViewTargetBlendFunction::VTBlend_Linear,0.f,true);
		}, 1.f, false);

}

void UOnGameEndedWidget::OnSpectateTeammateBtnClicked()
{
	CV(ABRPlayerController, Cont, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Cont)
		Cont->SpectateTeam();
}

void UOnGameEndedWidget::OnSpectateRandomBtnClicked()
{
	CV(ABRPlayerController, Cont, UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (Cont)
		Cont->SpectateRandom();
}

void UOnGameEndedWidget::OnLobyBtnClicked()
{
	RemoveFromParent();
	CV(UBRGameInstance, Inst, UGameplayStatics::GetGameInstance(GetWorld()));
	if (Inst)
		Inst->GoBackLobby();
}

void UOnGameEndedWidget::OnExitBtnClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
