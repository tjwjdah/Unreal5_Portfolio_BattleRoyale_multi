// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCapture2D.h"

#include "../../Items/PickupItem.h"
#include "../../Items/LootBox.h"
#include "../../Vehicles/Vehicle.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/PlayerStart.h"

AMapCapture2D::AMapCapture2D()
{

}

void AMapCapture2D::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(0.f, 0.f, 25000.f));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), ActorsToIgnore);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupItem::StaticClass(), ActorsToIgnore);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALootBox::StaticClass(), ActorsToIgnore);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicle::StaticClass(), ActorsToIgnore);
	
	GetCaptureComponent2D()->HiddenActors = ActorsToIgnore;
}
