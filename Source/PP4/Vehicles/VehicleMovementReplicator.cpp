// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMovementReplicator.h"
#include "Vehicle.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UVehicleMovementReplicator::UVehicleMovementReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);

	// ...
}


// Called when the game starts
void UVehicleMovementReplicator::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<AVehicle>(GetOwner());
	MovementComponent = GetOwner()->FindComponentByClass<UVehicleMovementComponent>();
	//MeshOffsetRoot = Owner->GetMeshOffsetRoot();
	

}


// Called every frame
void UVehicleMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!IsValid(Owner))
		Owner = Cast<AVehicle>(GetOwner());
	if (!IsValid(Owner)) return;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	return;
	if (MovementComponent == nullptr) return;
	
	FVehicleMove LastMove = MovementComponent->GetLastMove();
	
	//Owner->IsLocallyControlled() && !Owner->HasAuthority()
	//GetOwnerRole() == ROLE_AutonomousProxy
	/*
	if (Owner->IsLocallyControlled() && !Owner->HasAuthority()) {
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}
	//Owner->HasAuthority()
	//GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy
	if (Owner->IsLocallyControlled() && Owner->HasAuthority()) {
		UpdateServerState(LastMove);
	}
	
	//!Owner->IsLocallyControlled() && !Owner->HasAuthority()
	//GetOwnerRole() == ROLE_SimulatedProxy
	if (!Owner->IsLocallyControlled() && !Owner->HasAuthority()) {
		ClientTick(DeltaTime);
	}
	*/
	
	switch (Owner->GetLocalRole())
	{
	case ROLE_AutonomousProxy:
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
		break;
	case ROLE_Authority:
		if (Owner->IsLocallyControlled()) // 리슨 서버가 자기 폰을 직접 조종
		{
			MovementComponent->SimulateMove(LastMove); // 서버에서 실제 이동 적용
			UpdateServerState(LastMove);               // 복제 상태 갱신
		}
		break;
	case ROLE_SimulatedProxy:
		ClientTick(DeltaTime);
		break;
	}
	
	// ...
}


void UVehicleMovementReplicator::UpdateServerState(const FVehicleMove& Move)
{
	FVCState S;
	
	S.LastMove = Move;
	S.Tranform = Owner->GetActorTransform();
	S.Velocity = Owner->GetVelocity();

	ServerState = S;
}
void UVehicleMovementReplicator::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	FHermiteCubicSpline Spline = CreateSpline();

	InterpolateLocation(Spline, LerpRatio);

	InterpolateVelocity(Spline, LerpRatio);

	InterpolateRotation(LerpRatio);

}
FHermiteCubicSpline UVehicleMovementReplicator::CreateSpline()
{
	FHermiteCubicSpline Spline;
	Spline.TargetLocation = ServerState.Tranform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDerivative();
	Spline.TargetDerivative = ServerState.Velocity * VelocityToDerivative();
	return Spline;
}
void UVehicleMovementReplicator::InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRatio)
{
	FVector NewLocation = Spline.InterpolateLocation(LerpRatio);
	GetOwner()->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	/*
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldLocation(NewLocation);
	}
	*/
}
void UVehicleMovementReplicator::InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRatio)
{
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDerivative();
	MovementComponent->SetVelocity(NewVelocity);
}
void UVehicleMovementReplicator::InterpolateRotation(float LerpRatio)
{
	FQuat TargetRotation = ServerState.Tranform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();

	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);

	GetOwner()->SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);
	/*
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldRotation(NewRotation);
	}
	*/
}
float UVehicleMovementReplicator::VelocityToDerivative()
{
	return ClientTimeBetweenLastUpdates;
}

void UVehicleMovementReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UVehicleMovementReplicator, ServerState);
}


void UVehicleMovementReplicator::ClearAcknowledgeMoves(FVehicleMove LastMove)
{
	TArray<FVehicleMove> NewMoves;
	for (auto& Move : UnacknowledgedMoves) {
		if (LastMove.Time < Move.Time)
			NewMoves.Add(Move);
	}
	UnacknowledgedMoves = NewMoves;
}

void UVehicleMovementReplicator::OnRep_ServerState()
{

	if (!IsValid(Owner)) return;
	/*
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		Local_OnRep();
		break;
	case ROLE_SimulatedProxy:
		Simul_OnRep();
		break;
	default:
		break;
	}
	return;
	*/
	if (Owner->IsLocallyControlled())
	{
		Local_OnRep();
		return;
	}
	else
	{
		Simul_OnRep();
		return;
	}
	return;
}

void UVehicleMovementReplicator::Local_OnRep()
{
	if (MovementComponent == nullptr) return;
	/*
	const FTransform ServerT = ServerState.Tranform;
	const FTransform CurrT = GetOwner()->GetActorTransform();

	const float PosErr = FVector::Dist(ServerT.GetLocation(), CurrT.GetLocation());
	const float RotErr = FQuat::ErrorAutoNormalize(ServerT.GetRotation(), CurrT.GetRotation());

	const bool bDoSnap = (PosErr > 50.f) || (RotErr > 0.09f); // ~5도

	if (bDoSnap)
	{
		GetOwner()->SetActorTransform(ServerT);
		MovementComponent->SetVelocity(ServerState.Velocity);
		
	}*/

	GetOwner()->SetActorTransform(ServerState.Tranform,false, nullptr, ETeleportType::TeleportPhysics);
	MovementComponent->SetVelocity(ServerState.Velocity);


	ClearAcknowledgeMoves(ServerState.LastMove);

	for (const FVehicleMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UVehicleMovementReplicator::Simul_OnRep()
{
	if (MovementComponent == nullptr) return;

	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;
	/*
	if (MeshOffsetRoot != nullptr)
	{
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	*/
	//ClientStartVelocity = MovementComponent->GetVelocity();
	ClientStartVelocity = Owner->GetVelocity();
	ClientStartTransform = Owner->GetActorTransform();
	//GetOwner()->SetActorTransform(ServerState.Tranform);
}

void UVehicleMovementReplicator::Server_SendMove_Implementation(FVehicleMove _Move)
{
	if (MovementComponent == nullptr) return;

	ClientSimulatedTime += _Move.DeltaTime;
	MovementComponent->SimulateMove(_Move);
	
	UpdateServerState(_Move);
}

/*
bool UVehicleMovementReplicator::Server_SendMove_Validate(FVehicleMove _Move)
{
	return true;
	const float MaxClientLead = 0.25f; // 250ms 허용
	const float ProposedTime = ClientSimulatedTime + _Move.DeltaTime;
	const bool  ClientNotAhead = ProposedTime < (GetWorld()->TimeSeconds);
	if (!ClientNotAhead) {
		UE_LOG(LogTemp, Error, TEXT("Client is running too fast."))
			return false;
	}
	
	if(!_Move.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Received invalid move."))
			return false;
	}
	return true;
}
*/