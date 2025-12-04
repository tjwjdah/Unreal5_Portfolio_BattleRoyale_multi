// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMapLevelScript.h"
#include "../Main/BRGameInstance.h"
#include "GameFramework/PlayerStart.h"

void ALobbyMapLevelScript::BeginPlay()
{
	Super::BeginPlay();
	if (!IsRunningDedicatedServer() && UGameplayStatics::GetPlayerController(GetWorld(), 0)->IsLocalController()) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);
		FActorSpawnParameters	param;
		FTransform Trans(FRotator::ZeroRotator, FoundActors[0]->GetActorLocation(), FVector::One());
		
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		APawn* PreviewPawn = GetWorld()->SpawnActor<APawn>(GetRandomLobbyClass(),
			Trans,
			param);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(PreviewPawn);
		CV(UBRGameInstance, Instance, GetGameInstance());
		if (Instance) {
			Instance->SetLobyCharacterPreview(PreviewPawn);
		}
	}
}

TSubclassOf<APawn> ALobbyMapLevelScript::GetRandomLobbyClass()
{
	CV(UBRGameInstance, Instance, GetGameInstance());
	if (Instance->GetCharacterID() >= 0) {
		return Instance->GetLobbyCharacters()[Instance->GetCharacterID()];
	}
	else {
		Instance->SetCharacterID(0);
		return Instance->GetLobbyCharacters()[0];
	}
}
