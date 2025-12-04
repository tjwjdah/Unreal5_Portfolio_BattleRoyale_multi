// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"

#include "Net/UnrealNetwork.h"



ARocketLauncher::ARocketLauncher()
{
	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(GetStaticMesh());
}

void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void ARocketLauncher::Reload(int32 _AvailableAmmo, int32& _Used, int32& _RemainingAmmo)
{
	Super::Reload(_AvailableAmmo, _Used, _RemainingAmmo);
	McSetShowHideRocket(true);
}

void ARocketLauncher::MCPlaySoundandEffect_Implementation()
{
	Super::MCPlaySoundandEffect_Implementation();
	Rocket->SetHiddenInGame(true);
}

void ARocketLauncher::McSetShowHideRocket_Implementation(bool _Show)
{
	Rocket->SetHiddenInGame(!_Show);
}
