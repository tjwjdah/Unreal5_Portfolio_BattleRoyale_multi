// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosivesItem.h"

#include "../Character/BRCharacter.h"

#include "Net/UnrealNetwork.h"




void AExplosivesItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(AExplosivesItem, AttachSocketName);
	
}

void AExplosivesItem::BeginPlay()
{
	Super::BeginPlay();

	CV(ABRCharacter, Char, GetOwner());
	if (Char) {
		Box->SetSimulatePhysics(false);
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (IsValid(Char)) {
			FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			AttachToComponent(Char->GetMesh(), Rules, AttachSocketName);
		}
	}
}

void AExplosivesItem::SpawnExplosive(FVector _Location, FRotator _Rotator, AActor* _Actor)
{
	if (ExplosiveProjectile) {
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform Trans; Trans.SetLocation(_Location); Trans.SetRotation(_Rotator.Quaternion());
		AActor* Explosive= GetWorld()->SpawnActor<AActor>(ExplosiveProjectile, Trans, param);
		Explosive->SetOwner(_Actor);
	}
}
