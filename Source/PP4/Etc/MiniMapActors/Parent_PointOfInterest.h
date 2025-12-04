// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Parent_PointOfInterest.generated.h"

UCLASS()
class PP4_API AParent_PointOfInterest : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	FSlateColor Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	TObjectPtr<APawn> MyOwner;
public:	
	// Sets default values for this actor's properties
	AParent_PointOfInterest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FSlateColor GetColor() { return Color; }

	//void SetOwner(APawn* _Owner) { MyOwner = _Owner; }
	//void SetColor(FSlateColor _Color) { Color = _Color; }

	APawn* GetOwner() { return MyOwner; }

	UFUNCTION(Server, Reliable)
	void SERVERColor(FSlateColor _Color);

	UFUNCTION(Server, Reliable)
	void SERVEROwner(APawn* _Owner);
};
