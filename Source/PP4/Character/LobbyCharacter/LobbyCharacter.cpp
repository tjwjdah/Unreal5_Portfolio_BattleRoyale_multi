// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	CharacterMesh(TEXT("/Game/Assetss/7-Assets/Character/Mesh/SK_Mannequin"));

	// GetMesh() : Character클래스에 만들어져있는 SkeletalMeshComponent를 얻어오는 함수이다.
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(GetCapsuleComponent());

	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	Cylinder->SetupAttachment(GetCapsuleComponent());

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(Cylinder);

	Rifle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle"));
	Rifle->SetupAttachment(GetMesh(),TEXT("Weapon"));

	Rifle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle2"));
	Rifle2->SetupAttachment(GetMesh(), TEXT("WeapHolder2"));

	Rifle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle3"));
	Rifle3->SetupAttachment(GetMesh(), TEXT("WeapHolder3"));


		
}


// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	auto* Material = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0));
	if (Material) {
		Material->SetVectorParameterValue(TEXT("BodyColor"), Color);
		GetMesh()->SetMaterial(0, Material);
	}
}

// Called every frame
void ALobbyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALobbyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

