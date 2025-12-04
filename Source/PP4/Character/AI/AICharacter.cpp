// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "BRAIController.h"
#include "../BRCharacter.h"

#include "../../Main/BRGameMode.h"
#include "../../Main/BRGameState.h"
#include "../../Items/Bullet.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponDamage = 25.f;
	AIName = TEXT("AIBot");
	Health = 100.f;
	FireInterval = 1.f;
	MaxShots = 5.f;
	IsAllInZone = true;
	ActiveWeaponSlot = ESlotType::Primary;
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(GetMesh(), TEXT("Weapon"));
	DoOnce = true;

	static ConstructorHelpers::FObjectFinder<UAnimMontage>	Reload(TEXT("/Game/Assetss/2-BRCharacters/Animation/Combat/Reload"));

	if (Reload.Succeeded())
		ReloadMontage = Reload.Object;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakeAnyDamage.AddDynamic(this, &AAICharacter::OnAnyDamageReceived);
}

void AAICharacter::OnAnyDamageReceived(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsAlive())return;

	Health -= Damage;
	if (Health <= 0.f) {
		Health = 0.f;
		CV(ABRAIController, Cont, GetController());
		if (IsValid(Cont) && IsValid(Cont->GetBrainComponent())) {
			Cont->GetBrainComponent()->StopLogic(TEXT("Dead"));
			Cont->StopMovement();
		}
		MCAIDead();
	}
	CV(ABRCharacter, Char, DamageCauser);
	if (IsValid(Char)) {
		Char->CLIENTShowHitmarker();
		if (!IsAlive())
			Char->UpdateKillCount();
	}
	if (!IsAlive() && DoOnce) {
		DoOnce = false;
		CV(ABRGameMode, Mode, UGameplayStatics::GetGameMode(GetWorld()));
		CV(ACharacter, Ch, DamageCauser);
		Mode->UpdateKillFeed(this, Ch, UKismetSystemLibrary::GetDisplayName(DamageType));
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this] {
			if (IsValid(this))
			{
				this->Destroy();
			}
		}, 5.f, false);
		CV(ABRGameState, State, UGameplayStatics::GetGameState(GetWorld()));
		State->RemoveAIController(GetController());
		Mode->OnAIDeathCheckWinCondition();
	}

}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::InZone()
{
	CV(ABRAIController, Cont, GetController());
	if (IsValid(Cont)) {
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("NotInZone"), false);
		OnInOutZone(true);
		
	}
}

void AAICharacter::OutZone()
{
	CV(ABRAIController, Cont, GetController());
	if (IsValid(Cont)) {
		Cont->GetBlackboardComponent()->SetValueAsBool(TEXT("NotInZone"), true);
		OnInOutZone(false);
		Cont->ClearFocus(EAIFocusPriority::Default);
	}
}

void AAICharacter::OnInOutZone(bool _In)
{
	IsAllInZone = _In;
	GetCharacterMovement()->MaxWalkSpeed = _In ? 600.f : 900.f;
}

void AAICharacter::SpawnProjectile(FVector _Dest)
{
	FActorSpawnParameters	param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(),
		WeaponSkeletalMesh->GetSocketLocation(TEXT("Muzzle")), UKismetMathLibrary::FindLookAtRotation(WeaponSkeletalMesh->GetSocketLocation(TEXT("Muzzle")), _Dest), param);
	Bullet->SetOwner(this);
}

void AAICharacter::FireWeapon()
{
	if (!HasAuthority() || Reloading) return;

	if (ShotFired >= MaxShots) {
		Reload();
		return;
	}
	ShotFired++;
	MCFireWeapon();
	CV(ABRAIController, Cont, GetController());
	if (IsValid(Cont) && IsValid(Cont->GetCurrentTarget()))
	{
		FHitResult OutHit;
		FVector Start = GetMesh()->GetSocketLocation(TEXT("head"));
		FVector End = Cont->GetCurrentTarget()->GetMesh()->GetSocketLocation(TEXT("spine_03"));
		float Radius = 20.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(this); 

		bool bHit = GetWorld()->SweepSingleByChannel(
			OutHit,
			Start,
			End,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(Radius),
			QueryParams
		);
		SpawnProjectile(OutHit.TraceEnd);
	}
	if (ShotFired >= MaxShots)
		Reload();
}
void AAICharacter::MCFireWeapon_Implementation()
{
	if (IsRunningDedicatedServer())return;
	StopAnimMontage();
	UGameplayStatics::SpawnSoundAttached(WeaponSound, WeaponSkeletalMesh);
	auto* Flash= UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, WeaponSkeletalMesh, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, Flash] {
		if (IsValid(Flash))
		{
			Flash->DestroyComponent();
		}
		}, 0.2f, false);

}
void AAICharacter::Reload()
{
	Reloading = true;
	MCReload();
	
}

void AAICharacter::MCCharacterHit_Implementation(FVector _Location, bool _CharacterHit)
{
	FTransform Trans; Trans.SetLocation(_Location); Trans.SetScale3D(FVector(1.5f, 1.5f, 1.5f));
	UParticleSystem* ParticleTemplate = LoadObject<UParticleSystem>
		(nullptr, TEXT("ParticleSystem'/Game/Assetss/7-Assets/Effects/ParticleSystems/BloodParticle/ParticleSystems/Gameplay/Player/P_body_bullet_impact.P_body_bullet_impact'"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), nullptr, Trans);
}

void AAICharacter::MCReload_Implementation()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	Reloading = false;
	ShotFired = 0;
}


void AAICharacter::MCAIDead_Implementation()
{
	StopAnimMontage();
	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true);
}


void AAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제할 변수 추가
	DOREPLIFETIME(AAICharacter, ActiveWeaponSlot);

}