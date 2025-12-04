// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneMapLevelScript.h"
#include "../Main/BRPlayerController.h"

void APlaneMapLevelScript::BeginPlay()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlaneMapLevelScript::MinimapSetting, 1.f, false);

}

void APlaneMapLevelScript::MinimapSetting()
{
	if (!IsRunningDedicatedServer() && UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController()) {
		CV(ABRPlayerController, Controller, UGameplayStatics::GetPlayerController(GetWorld(), 0));
		ifv(Controller) {
			FMinimapSetting Setting;
			Setting.Texture= LoadObject<UTexture2D>(nullptr,
				TEXT("Texture2D'/Game/Assetss/5-Minimap/MinimapComponent/Actors/MapCapture2D/Minimap450K.Minimap450K'"));
			Setting.CapturedMapSizeInWorldUnits = 450000.f;
			Setting.Zoom = 0.25f;
			Controller->SetMinimapSettings(Setting);
		}
	}
}
