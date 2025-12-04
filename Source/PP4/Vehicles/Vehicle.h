// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

#include "VehicleMovementComponent.h"
#include "VehicleMovementReplicator.h"
#include "WheeledVehiclePawn.h"
#include "Vehicle.generated.h"

/**
 * 
 */
UCLASS()
class PP4_API AVehicle : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	AVehicle();
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> InternalCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> ChaseCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextRenderComponent> CarGear;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextRenderComponent> CarSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsInCar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor GearDisplayColor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor GearDisplayReverseColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString SpeedDisplayString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString GearDisplayString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool ReverseGear;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString VehicleName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool Steat1Used;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool Steat2Used;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool Steat3Used;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Replicated)
	bool Steat4Used;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UVehicleMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UVehicleMovementReplicator* MovementReplicator;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABRCharacter> SeatOwner1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABRCharacter> SeatOwner2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABRCharacter> SeatOwner3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABRCharacter> SeatOwner4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABRCharacter> LocalOwner;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> MeshOffsetRoot;
	


	bool Gate1;
	bool Gate2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Scale);

	void MoveRight(float Scale);

	void Turn(float Scale);

	void LookUp(float Scale);

	void ChangeCamera();
	
	void Interact();

	void PressedBrake();
	void ReleasedBrake();

	void OpenGate(int32 _num) {
		if (_num == 1)Gate1 = true;
		else if (_num == 2) Gate2 = true;
	}
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsSeatEmpty();
	FString GetCarName() { return VehicleName; }
	void SetSeatOwner(EVehicleSeats _Seat, class ABRCharacter* _Character);
	void SetLocalOwner(ABRCharacter* _Owner) { LocalOwner = _Owner; };
	EVehicleSeats GetEmptySeat(FName& _SeatSocket);
	
	ABRCharacter* GetSeatOwner(int32 _num);
	
	void UpdateHUDStrings();

	void SetupIncarHUD();

	void EnableCarView(bool _State);

	FVector FindEmptySpot(EVehicleSeats _Seat);

	
	virtual void NotifyHit(UPrimitiveComponent* MyComp,AActor* Other,UPrimitiveComponent* OtherComp,bool bSelfMoved,FVector HitLocation,FVector HitNormal,FVector NormalImpulse,const FHitResult& Hit) override;

	UFUNCTION(Server, Reliable)
	void SERVERRequestExit(ABRCharacter* _Owner);


	USceneComponent* GetMeshOffsetRoot() { return MeshOffsetRoot; }
};
