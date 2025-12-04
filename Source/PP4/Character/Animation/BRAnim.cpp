// Fill out your copyright notice in the Description page of Project Settings.


#include "BRAnim.h"

#include "../BRCharacter.h"
#include "../AI/AICharacter.h"

#include "../../Items/WeaponItem.h"

#include "Kismet/KismetMathLibrary.h"


void UBRAnim::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    if (!TryGetPawnOwner())
        return;
    CV(ABRCharacter, Char, TryGetPawnOwner());
    if (IsValid(Char))
        MyChar = Char;
    else
    {
        CV(AAICharacter, AIChar, TryGetPawnOwner());
        if (IsValid(AIChar))
            AICharacter = AIChar;
    }
    HandOnGunT.SetLocation(FVector(-28.271578f, 13.630774f, 0.520170f));
}

void UBRAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    if (!IsValid(MyChar))
    {
       // if (IsValid(AICharacter))
          //  UpdateAIInfo();
        return;
    }

    IsInAir= MyChar->GetMovementComponent()->IsFalling();
   
    FVector xy(MyChar->GetVelocity().X, MyChar->GetVelocity().Y, 0.f);
    Speed = xy.Size();
    Direction = Speed > 0.f ? UKismetMathLibrary::NormalizedDeltaRotator(xy.ToOrientationRotator(), MyChar->GetRepControl()).Yaw : 0.f;
    Movement = MyChar->GetPlayerInfo().Movement;
    CurrentActiveSlot = MyChar->GetRepActiveSlot();
    Firing = MyChar->GetIsFiring();
    Aiming = Firing ? true : MyChar->GetAiming();
    RotatingInPlace = MyChar->GetRotatingInPlace();
    Pitch = FMath::Clamp(MyChar->GetAimPitch(), -70.f, 70.f);
    Yaw = FMath::FInterpTo(Yaw, FMath::Clamp(MyChar->GetAimYaw(), -85.f, 85.f), DeltaSeconds, 8.f);
    UnarmedYaw = Speed > 0.f ? 0.f : Yaw;
    Crouching = MyChar->GetCharacterMovement()->IsCrouching();
    ForwardBtnDown = MyChar->GetBtnFwdDown();
    LeftBtnDown = MyChar->GetBtnLeftDown();
    RightBtnDown = MyChar->GetBtnRightDown();
    WalkingState = MyChar->GetWalkingState();
    IsParachuting = MyChar->GetParachuting();

  
    Leaning = FMath::FInterpTo(Leaning, MyChar->GetRepLeaning(), DeltaSeconds, 5.f);
    LeaningSpine1.Pitch = Leaning * MyChar->GetLeaningSettings().Spine1;
    LeaningSpine2.Pitch = Leaning * MyChar->GetLeaningSettings().Spine2;
    LeaningSpine3.Pitch = Leaning * MyChar->GetLeaningSettings().Spine3;

    IsDriver = MyChar->GetIsVehicleDriver();

    IsPlayerDown = MyChar->GetIsPlayerDown();

}
void UBRAnim::AnimNotify_ThrowGrenade()
{
    CV(ABRCharacter, Player, TryGetPawnOwner());

    if (Player && Player->IsLocallyControlled())
        Player->SERVERThrowGrenade();
}

void UBRAnim::AnimNotify_RifleReload()
{
    CV(ABRCharacter, Player, TryGetPawnOwner());

    if (Player && Player->IsLocallyControlled()) {
        Player->SERVERReloadWeapon();
    }
}
void UBRAnim::AnimNotify_PistolReload()
{
    CV(ABRCharacter, Player, TryGetPawnOwner());

    if (Player && Player->IsLocallyControlled()) {
        Player->SERVERReloadWeapon();
    }
}

void UBRAnim::AnimNotify_EquipWeapon()
{
    CV(ABRCharacter, Player, TryGetPawnOwner());
   
    if (Player && Player->IsLocallyControlled()) {
        Player->SetEquipMontageOn(true);
        Player->SERVERChangeSlot(Player->GetChangeSlot());
    }
}

void UBRAnim::UpdateAIInfo()
{
    Speed = AICharacter->GetVelocity().Length();
    IsInAir = AICharacter->GetMovementComponent()->IsFalling();
    Crouching = AICharacter->GetMovementComponent()->IsCrouching();
    CurrentActiveSlot = AICharacter->GetActiveWeaponSlot();
    Direction = Speed > 0.f ? CalculateDirection(AICharacter->GetVelocity(), AICharacter->GetActorRotation()) : 0.f;
}

void UBRAnim::SelectPrimaryAnimation()
{
    if (CurrentEquipWeaponID != TEXT("MachineGun") && CurrentEquipWeaponID != TEXT("SMG") && !IsValid(RifleAnimationAsset) ) {
       
    }
 
}
