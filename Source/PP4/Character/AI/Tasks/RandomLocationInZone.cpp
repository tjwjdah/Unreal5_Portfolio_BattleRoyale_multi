// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomLocationInZone.h"

#include "../../../World/Zone.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

URandomLocationInZone::URandomLocationInZone()
{
    NodeName = TEXT("RandomLocationInZone");
    bNotifyTick = false; // TickTask ¾È ¾¸
}

URandomLocationInZone::~URandomLocationInZone()
{
}

EBTNodeResult::Type URandomLocationInZone::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* Comp = OwnerComp.GetBlackboardComponent();
    if (!IsValid(Comp)) return EBTNodeResult::Succeeded;

    UObject* ZoneObj = Comp->GetValueAsObject(Zone.SelectedKeyName);
    CV(AZone, Zoneptr, ZoneObj);
   
    if (!IsValid(Zoneptr)) return EBTNodeResult::Succeeded;

    FVector Origin;
    FVector BoxExtent;
    Zoneptr->GetActorBounds(false, Origin, BoxExtent);

    FVector Center = Zoneptr->GetActorLocation();
    float Radius = BoxExtent.X;

    FNavLocation Location;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (IsValid(NavSys) && NavSys->GetRandomPointInNavigableRadius(Center, Radius, Location))
    {
        Comp->SetValueAsVector(Key.SelectedKeyName, Location.Location);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Succeeded;
}
