// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosives.h"
#include "DamageTypes/ExplosionDamage.h"
#include "../Character/BRCharacter.h"
#include "../Character/AI/AICharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"


AExplosives::AExplosives()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetStaticMeshComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	SetReplicates(true);

	ProjectileMovement->InitialSpeed = 2500.f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.35;
	ProjectileMovement->Friction = 0.6;

	
}

void AExplosives::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this] {
			Explosion();
			}, 3.0f, false);
	
}


void AExplosives::Explosion()
{
	CV(ABRCharacter, Char, GetOwner());
	if (!IsValid(Char))
	{
		Destroy();
		return;
	}
	OwnerPawn = Char;
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
	FTransform Trans;
	Trans.SetLocation(GetActorLocation()); Trans.SetScale3D(FVector(3.f, 3.f, 3.f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterImpact, Trans);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());

	if (HasAuthority()) {
		FCollisionQueryParams Params;
		Params.bTraceComplex = false;
		Params.AddIgnoredActor(this);

		TArray<FHitResult> OutHits;
		GetWorld()->SweepMultiByChannel(
			OutHits,
			GetActorLocation(),
			GetActorLocation()+FVector(1.f,1.f,1.f),
			FQuat::Identity,
			ECC_GameTraceChannel2,   // Trace Channel
			FCollisionShape::MakeSphere(500.f),
			Params
		);
		for (int32 i = 0; i < OutHits.Num(); i++) {
			CV(ABRCharacter, Char1, OutHits[i].GetActor());
			CV(AAICharacter,Char2, OutHits[i].GetActor());
			if (IsValid(Char1)) {
				Char1->DamageTaken(BaseDamage, TEXT("Explosion"), IsValid(OwnerPawn) ? OwnerPawn : nullptr, false);
			}
			else if (IsValid(Char2)) {
				UGameplayStatics::ApplyDamage(Char2, BaseDamage, nullptr, OwnerPawn, UExplosionDamage::StaticClass());
			}
		}
	}
}
