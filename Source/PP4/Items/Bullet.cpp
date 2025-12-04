// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "PickupItem.h"
#include "DamageTypes/FirearmDamage.h"

#include "../Character/BRCharacter.h"

#include "Sound/SoundCue.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	InitialLifeSpan = 3.f;
	
	SetReplicates(true);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	SetCanBeDamaged(false);

	ProjectileMovement->SetUpdatedComponent(Sphere);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Sphere);
	
	static ConstructorHelpers::FObjectFinder<USoundCue> HitSoundCue = (TEXT("/Game/Assetss/7-Assets/Sounds/Weapons/BulletHit_Cue"));
	if (HitSoundCue.Succeeded())
	{
		HitSound = HitSoundCue.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> OnHitEffectParticle = (TEXT("/Game/Assetss/7-Assets/StarterContent/Explosion/P_Sparks"));
	if (OnHitEffectParticle.Succeeded())
	{
		OnHitEffect = OnHitEffectParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> OnBloodHitEffectParticle = (TEXT("/Game/Assetss/7-Assets/Effects/ParticleSystems/BloodParticle/ParticleSystems/Gameplay/Player/P_body_bullet_impact"));
	if (OnBloodHitEffectParticle.Succeeded())
	{
		OnBloodHitEffect = OnBloodHitEffectParticle.Object;
	}
	BodyArmorBones.Add(TEXT("spine_01"));
	BodyArmorBones.Add(TEXT("spine_02"));
	BodyArmorBones.Add(TEXT("spine_03"));
	BodyArmorBones.Add(TEXT("clavicle_l"));
	BodyArmorBones.Add(TEXT("clavicle_r"));
	BodyArmorBones.Add(TEXT("UpperArm_L"));
	BodyArmorBones.Add(TEXT("UpperArm_R"));

	ProjectileMovement->ProjectileGravityScale =0.2f;
	ProjectileMovement->Velocity = FVector(22000.f, 0.f, 0.f);
	Sphere->SetSphereRadius(2.f);
	Sphere->SetNotifyRigidBodyCollision(true);

	Sphere->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABullet::GetDamageMultiplier(FName _BoneName, ABRCharacter* _Char)
{
	float DamageTaken; float ArmorRemaining;
	if (_BoneName == TEXT("Head")) {
		if (_Char->GetHeadArmorInfo().HasArmor) {
			CalculateArmorDamage(BaseDamage * 2, _Char->GetHeadArmorInfo().ArmorValue, DamageTaken, ArmorRemaining);
			_Char->UpdateArmor(ESlotType::HeadArmor, ArmorRemaining);
			return DamageTaken;
		}
		else
			return BaseDamage * 2;
	}
	else {
		if (BodyArmorBones.Contains(_BoneName) &&_Char->GetBodyArmorInfo().HasArmor) {
			CalculateArmorDamage(BaseDamage, _Char->GetBodyArmorInfo().ArmorValue, DamageTaken, ArmorRemaining);
			_Char->UpdateArmor(ESlotType::BodyArmor, ArmorRemaining);
			return DamageTaken;
		}
		else
			return BaseDamage;
	}
	return 0.0f;
}

void ABullet::CalculateArmorDamage(float _Damage, float _Armor, float& _DamageTaken, float& _ArmorRemaining)
{
	if (_Damage > _Armor) {
		_DamageTaken =_Damage-_Armor;
		_ArmorRemaining = 0.f;
	}
	else if (_Damage == _Armor) {
		_DamageTaken = 0.f;
		_ArmorRemaining = 0.f;
	}
	else{
		_DamageTaken = 0.f;
		_ArmorRemaining = _Armor - _Damage;
	}
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location);
	CV(ACharacter, Char, Hit.GetActor());
	CV(ABRCharacter, BRChar, Hit.GetActor());
	CV(ACharacter, OnwerChar, GetOwner());
	MCOnHit(Hit.Location, IsValid(Char));
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (HasAuthority()) {
		if (!IsValid(BRChar)) {
			if (IsValid(Hit.GetActor())) {
				UGameplayStatics::ApplyDamage(Hit.GetActor(), BaseDamage, nullptr, OnwerChar, UFirearmDamage::StaticClass());
			}
		}
		else {
			if (BRChar != OnwerChar && BaseDamage > 0.f && IsValid(OnwerChar)) {
				CV(APickupItem, Item, Weapon);
				if (IsValid(Item))
					BRChar->DamageTaken(GetDamageMultiplier(Hit.BoneName, BRChar), Item->GetInfo().Name.ToString(), OnwerChar, true);
			}
		}
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this] {
			Destroy();
			}, 1.0f, false);
	}
	SetActorHiddenInGame(true);
}

void ABullet::MCOnHit_Implementation(FVector _Location, bool _Hit)
{
	if (IsValid(_Hit ? OnBloodHitEffect : OnHitEffect))
	{
		FTransform Trans;
		Trans.SetLocation(_Location);
		Trans.SetScale3D(FVector(2.0f, 2.0f, 2.0f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _Hit ? OnBloodHitEffect : OnHitEffect, Trans);
	}
}

