// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

#include "../Character/BRCharacter.h"

#include "../Items/DamageTypes/MeleeDamage.h"

#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Components/TextRenderComponent.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"

AVehicle::AVehicle()
{
    SetReplicateMovement(false);


    /*
    MeshOffsetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshOffsetRoot"));
    MeshOffsetRoot->SetupAttachment(Box);

    GetMesh()->SetupAttachment(MeshOffsetRoot);
    */
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetMesh());

    ChaseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
    ChaseCamera->SetupAttachment(SpringArm);

    CarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CarGear"));
    CarGear->SetupAttachment(GetMesh());

    CarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CarSpeed"));
    CarSpeed->SetupAttachment(GetMesh());

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    Scene->SetupAttachment(GetMesh());

    InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
    InternalCamera->SetupAttachment(Scene);
    
    GearDisplayColor = FColor::White;
    GearDisplayReverseColor = FColor::Red;
    
    VehicleName = TEXT("Car");
    
    GetMesh()->SetNotifyRigidBodyCollision(true);

  
    Gate1 = true;    
    Gate2 = true;

    bReplicates = true;
    SetReplicateMovement(false);

    MovementComponent = CreateDefaultSubobject<UVehicleMovementComponent>(TEXT("MovementComponent"));
    MovementReplicator = CreateDefaultSubobject<UVehicleMovementReplicator>(TEXT("MovementReplicator"));
    
}

void AVehicle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // 복제할 변수 추가
    DOREPLIFETIME(AVehicle, Steat1Used);
    DOREPLIFETIME(AVehicle, Steat2Used);
    DOREPLIFETIME(AVehicle, Steat3Used);
    DOREPLIFETIME(AVehicle, Steat4Used);
}
void AVehicle::BeginPlay()
{
    Super::BeginPlay();
}
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    //
    PlayerInputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AVehicle::ChangeCamera);
    PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Pressed, this, &AVehicle::PressedBrake);
    PlayerInputComponent->BindKey(EKeys::SpaceBar, IE_Released, this, &AVehicle::ReleasedBrake);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AVehicle::Interact);

    PlayerInputComponent->BindAxis("MoveForward", this, &AVehicle::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AVehicle::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AVehicle::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &AVehicle::LookUp);
}
void AVehicle::MoveForward(float Scale)
{
   
    if (MovementComponent == nullptr) return;

    MovementComponent->SetThrottle(Scale);
    
    return;
    /*
    auto* M = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
    if (!M) return;

    if (Scale >= 0.f)
    {
      
        M->SetBrakeInput(0.f);
        M->SetThrottleInput(FMath::Clamp(Scale, 0.f, 1.f));
      
    }
    else
    {
       const float a = FMath::Clamp(-Scale, 0.f, 1.f);
       M->SetBrakeInput(a);
       M->SetThrottleInput(0.f);
    }
    */
}
void AVehicle::MoveRight(float Scale)
{
   
    if (MovementComponent == nullptr) return;

    MovementComponent->SetSteeringThrow(Scale);
    /*
    return;
    GetVehicleMovement()->SetSteeringInput(Scale);
    */

}
void AVehicle::Turn(float Scale)
{
    AddControllerYawInput(Scale);
}
void AVehicle::LookUp(float Scale)
{
    AddControllerPitchInput(Scale);
}
void AVehicle::ChangeCamera()
{
    EnableCarView(!IsInCar);
}
void AVehicle::Interact()
{
    if (Gate1) {
    SERVERRequestExit(LocalOwner);
    Gate1 = false;
    }
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, [this] {
        OpenGate(1);
        }, 0.2f, false);
}
void AVehicle::PressedBrake()
{
    
    if (MovementComponent == nullptr) return;

    MovementComponent->SetHandbrake(true);
    MovementComponent->SetBrake(1);
    MovementComponent->SetThrottle(0.f);

    return;
    /*
    GetVehicleMovement()->SetHandbrakeInput(true);
    GetVehicleMovement()->SetBrakeInput(1);
    GetVehicleMovement()->SetThrottleInput(0.f);
    */
}
void AVehicle::ReleasedBrake()
{
   
    if (MovementComponent == nullptr) return;

    MovementComponent->SetHandbrake(false);

    return;
   // GetVehicleMovement()->SetHandbrakeInput(false);

   
}
void AVehicle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    /*
    if (auto* Move = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement()))
    {

        const float Throttle = Move->GetThrottleInput();
        const float Steering = Move->GetSteeringInput();
        const float Brake = Move->GetBrakeInput();
        const bool  HB = Move->GetHandbrakeInput();
        const int32 Gear = Move->GetCurrentGear();
        const float Speed = Move->GetForwardSpeed();          // cm/s (5.3)

        // 엔진 회전수 (rad/s → RPM 변환)
        const float RotRad = Move->GetEngineRotationSpeed();   // 5.3 API
        const float RPM = RotRad * 60.f / (2.f * PI);

        // 접지 여부: 모든 휠 검사해서 하나라도 접지면 지상
        bool bAir = true;
        const int32 NumWheels = Move->GetNumWheels();
        for (int32 i = 0; i < NumWheels; ++i)
        {
            const FWheelStatus& S = Move->GetWheelState(i);      // 5.3 API
            if (S.bInContact) { bAir = false; break; }           // 접지 플래그
        }

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green,
                FString::Printf(TEXT("Thr=%.2f Ste=%.2f  Br=%.2f HB=%d Gear=%d Spd=%.0f RPM=%.0f Air=%d"),
                    Throttle, Steering, Brake, HB ? 1 : 0, Gear, Speed, RPM, bAir ? 1 : 0));
        }
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green,
            FString::Printf(TEXT("X=%.2f Y=%.2f  Z=%.2f"),
                GetVelocity().X, GetVelocity().Y, GetVelocity().Z));
    }
    */
    ReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
    UpdateHUDStrings();
    SetupIncarHUD();
    if (!IsInCar)return;

    FRotator Rot(
    UKismetMathLibrary::ClampAngle(InternalCamera->GetRelativeRotation().Pitch + GetInputAxisValue(TEXT("LookUp")), -89, 89),
    InternalCamera->GetRelativeRotation().Yaw + GetInputAxisValue(TEXT("Turn")), 0.f);
    InternalCamera->SetRelativeRotation(Rot);
}
bool AVehicle::IsSeatEmpty()
{
    return (!Steat1Used || !Steat2Used || !Steat3Used || !Steat4Used) ;
}


void AVehicle::SetSeatOwner(EVehicleSeats _Seat, ABRCharacter* _Character)
{
    
    if (EVehicleSeats::Seat1 == _Seat) {
        SeatOwner1 = _Character;
        Steat1Used = IsValid(_Character);
    } 
    else if(EVehicleSeats::Seat2 == _Seat) {
        SeatOwner2 = _Character;
        Steat2Used = IsValid(_Character);
    }
    else if (EVehicleSeats::Seat3 == _Seat) {
        SeatOwner3 = _Character;
        Steat3Used = IsValid(_Character);
    }
    else if (EVehicleSeats::Seat4 == _Seat) {
        SeatOwner4 = _Character;
        Steat4Used = IsValid(_Character);
    }

}

EVehicleSeats AVehicle::GetEmptySeat(FName& _SeatSocket)
{
    if (!Steat1Used) {
        _SeatSocket = TEXT("Seat1");
        return EVehicleSeats::Seat1;
    }
    else if (!Steat2Used) {
        _SeatSocket = TEXT("Seat2");
        return EVehicleSeats::Seat2;
    }
    else if (!Steat3Used) {
        _SeatSocket = TEXT("Seat3");
        return EVehicleSeats::Seat3;
    }
    else {
        _SeatSocket = TEXT("Seat4");
        return EVehicleSeats::Seat4;
    }
}


ABRCharacter* AVehicle::GetSeatOwner(int32 _num = 1)
{
    if (_num == 1) {
        return SeatOwner1;
    }
    else  if (_num == 2) {
        return SeatOwner2;
    }
    else  if (_num == 3) {
        return SeatOwner3;
    }
    else  if (_num == 4) {
        return SeatOwner4;
    }

    return SeatOwner1;
}

void AVehicle::UpdateHUDStrings()
{
    SpeedDisplayString = FString::FromInt(FMath::FloorToInt32(abs(GetVehicleMovement()->GetForwardSpeed() * 0.036))) + TEXT("km/h");
    if (ReverseGear) {
        GearDisplayString = TEXT("R");
    }
    else {
        if (GetVehicleMovement()->GetCurrentGear() > 0)
            GearDisplayString = FString::FromInt(GetVehicleMovement()->GetCurrentGear());
        else
            GearDisplayString = TEXT("N");
    }
   
}

void AVehicle::SetupIncarHUD()
{
    CarGear->SetText(FText::FromString(GearDisplayString));
    CarGear->SetTextRenderColor((ReverseGear ? GearDisplayReverseColor : GearDisplayColor).ToFColor(true));
    CarSpeed->SetText(FText::FromString(SpeedDisplayString));
}

void AVehicle::EnableCarView(bool _State)
{
    IsInCar = _State;
    if (IsInCar) {
        InternalCamera->Activate(false);
        ChaseCamera->Deactivate();
    }
    else {
        InternalCamera->Deactivate();
        ChaseCamera->Activate(false);
    }
    CarGear->SetVisibility(IsInCar);
    CarSpeed->SetVisibility(IsInCar);
}

FVector AVehicle::FindEmptySpot(EVehicleSeats _Seat)
{
   
    FVector End = GetActorLocation() + (UKismetMathLibrary::GetRightVector(GetActorRotation()) * ((_Seat == EVehicleSeats::Seat1 || _Seat == EVehicleSeats::Seat3) ? -200.f : 200.f));
    FVector Loc(0.f, 0.f, 50.f);
    FHitResult OutHit;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.AddIgnoredActor(SeatOwner1);
    CollisionParams.AddIgnoredActor(SeatOwner2);
    CollisionParams.AddIgnoredActor(SeatOwner3);
    CollisionParams.AddIgnoredActor(SeatOwner4);
    CollisionParams.AddIgnoredActor(LocalOwner);
    CollisionParams.bTraceComplex = false;

    bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit,GetActorLocation()+Loc,End+Loc,
        ECollisionChannel::ECC_Visibility, CollisionParams);
    if (!bHit)
        return OutHit.TraceEnd;

    End = GetActorLocation() + (UKismetMathLibrary::GetForwardVector(GetActorRotation()) * -300.f);
    bHit = GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation() + Loc, End + Loc,
        ECollisionChannel::ECC_Visibility, CollisionParams);
    if (!bHit)
        return OutHit.TraceEnd;

    End = GetActorLocation() + (UKismetMathLibrary::GetForwardVector(GetActorRotation()) * 300.f);
    bHit = GetWorld()->LineTraceSingleByChannel(OutHit, GetActorLocation() + Loc, End + Loc,
        ECollisionChannel::ECC_Visibility, CollisionParams);
    if (!bHit)
        return OutHit.TraceEnd;

    return GetActorLocation() + FVector(0.f, 0.f, 500.f);
}

void AVehicle::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority())return;
    if (bSelfMoved && abs(GetVehicleMovement()->GetForwardSpeed()) > 50.f) {
        CV(ACharacter, Char, Other);
        if (!IsValid(Char)) return;
        if (Char != SeatOwner1 && Char != SeatOwner2 && Char != SeatOwner3 && Char != SeatOwner4) {
            if (Gate2) {
                Gate2 = false;
                Char->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 500.f,true);
                CV(ABRCharacter, Char1, Other);
                if (IsValid(Char1))
                    Char1->DamageTaken(GetVelocity().Size() > 300.f ? 100.f : 30.f, VehicleName, SeatOwner1, false);
                else
                    UGameplayStatics::ApplyDamage(Char, GetVelocity().Size() > 300.f ? 100.f : 30.f, nullptr, SeatOwner1, UMeleeDamage::StaticClass());
            }
            FTimerHandle TimerHandle;
            GetWorldTimerManager().SetTimer(TimerHandle, [this] {
                OpenGate(2);
                }, 0.1f, false);
        }
    }
}

void AVehicle::SERVERRequestExit_Implementation(ABRCharacter* _Owner)
{
    _Owner->ServerHandleExitingVehicle(this);
}
