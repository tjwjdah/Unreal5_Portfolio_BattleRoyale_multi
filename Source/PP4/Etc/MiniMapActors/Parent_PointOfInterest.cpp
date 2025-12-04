// Fill out your copyright notice in the Description page of Project Settings.


#include "Parent_PointOfInterest.h"

#include "Net/UnrealNetwork.h"
// Sets default values
AParent_PointOfInterest::AParent_PointOfInterest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void AParent_PointOfInterest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(AParent_PointOfInterest, Color);
	DOREPLIFETIME(AParent_PointOfInterest, MyOwner);
	
}
// Called when the game starts or when spawned
void AParent_PointOfInterest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParent_PointOfInterest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParent_PointOfInterest::SERVEROwner_Implementation(APawn* _Owner)
{
	MyOwner = _Owner;
}

void AParent_PointOfInterest::SERVERColor_Implementation(FSlateColor _Color)
{
	Color = _Color;
}

