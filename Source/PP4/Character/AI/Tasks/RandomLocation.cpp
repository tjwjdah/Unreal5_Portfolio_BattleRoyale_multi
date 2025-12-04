// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomLocation.h"
#include "AIController.h"

#include "../../../World/Zone.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


URandomLocation::URandomLocation()
{
	NodeName = TEXT("RandomLocation");
	bNotifyTick = false; 
    Radius = 5000.f;
}

URandomLocation::~URandomLocation()
{
}

EBTNodeResult::Type URandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* Comp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(Comp)) return EBTNodeResult::Succeeded;

    UObject* ZoneObj = Comp->GetValueAsObject(Zone.SelectedKeyName);
    CV(AZone, Zoneptr, ZoneObj);
    
    FVector Origin;
    FVector BoxExtent;
    FVector Center;
    if (IsValid(Zoneptr)) {
        Zoneptr->GetActorBounds(false, Origin, BoxExtent);
        Radius = BoxExtent.X;
        Center = Zoneptr->GetActorLocation();
    }
    else {
        AAIController* AIController = OwnerComp.GetAIOwner();
        if (!IsValid(AIController)) return EBTNodeResult::Succeeded;
        if(!IsValid(AIController->GetPawn()))return EBTNodeResult::Succeeded;
        AIController->GetPawn()->GetActorBounds(false, Origin, BoxExtent);
        Center = AIController->GetPawn()->GetActorLocation();
    }
  

    FNavLocation Location;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (IsValid(NavSys) && NavSys->GetRandomPointInNavigableRadius(Center, Radius, Location))
    {
        Comp->SetValueAsVector(Key.SelectedKeyName, Location.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Succeeded;
}
