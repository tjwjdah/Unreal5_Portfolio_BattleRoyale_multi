// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeBullet.h"

#include "../../Character/BRCharacter.h"

#include "../DamageTypes/ExplosionDamage.h"
AGrenadeBullet::AGrenadeBullet()
{
	GrenadeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeStaticMesh"));

	GrenadeStaticMesh->SetupAttachment(GetSphereComponent());
	GetSphereComponent()->OnComponentHit.RemoveDynamic(this, &ABullet::OnHit);
	GetSphereComponent()->OnComponentHit.AddDynamic(this, &AGrenadeBullet::OnGrenadeHit);
	DoOnce = true;
}

void AGrenadeBullet::OnGrenadeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!DoOnce)return;
	DoOnce = false;
	CV(APawn, Pawn, Hit.GetActor())
		if (Hit.bBlockingHit)
			MCOnHit(FVector::ZeroVector, IsValid(Pawn));

	GetSphereComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(false);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterImpact, Hit.Location, FRotator::ZeroRotator, FVector(3.f, 3.f, 3.f));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GetHitSound(), Hit.Location);

	if (!HasAuthority())return;

	FCollisionQueryParams Params;
	Params.bTraceComplex = false;
	Params.AddIgnoredActor(this);

	TArray<FHitResult> OutHits;
	GetWorld()->SweepMultiByChannel(
		OutHits,
		Hit.Location,
		Hit.Location + FVector(1.f, 1.f, 1.f),
		FQuat::Identity,
		ECC_GameTraceChannel2,   // Trace Channel
		FCollisionShape::MakeSphere(500.f),
		Params
	);
	CV(ACharacter, OwnerChar, GetOwner());
	for (int32 i = 0; i < OutHits.Num(); i++) {
		if (!OutHits[i].bBlockingHit)continue;
		CV(ABRCharacter, Char, OutHits[i].GetActor());
		if (IsValid(Char)) {
			if (IsValid(GetOwner()))
				Char->DamageTaken(GetBaseDamage(),TEXT("Explosion"), OwnerChar,false);
		}
		else {
			if (IsValid(OutHits[i].GetActor()))
				UGameplayStatics::ApplyDamage(OutHits[i].GetActor(), GetBaseDamage(), nullptr, OwnerChar, UExplosionDamage::StaticClass());
		}
	}
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this] {
		Destroy();
		}, 1.0f, false);
}

