// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypointTaskNode.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "PatrolComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"



EBTNodeResult::Type UChooseNextWaypointTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
    auto index = blackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
    //UE_LOG(LogTemp, Warning, TEXT("Waypoint key name and index: %s: %i"), *(IndexKey.SelectedKeyName.ToString()), index);

    AAIController* aiController = OwnerComp.GetAIOwner();

    if (aiController)
    {
        auto controlledPawn = aiController->GetPawn();

        if (controlledPawn)
        {
            auto component = Cast<UPatrolComponent>(controlledPawn->FindComponentByClass<UPatrolComponent>());
            if (component)
            {
                TArray<AActor*> patrolPoints = component->GetPatrolPoints();
                if (patrolPoints.Num() > 0)
                {
                    blackboardComp->SetValueAsObject(ObjectKey.SelectedKeyName, patrolPoints[index]);
                    blackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, (index + 1) % patrolPoints.Num());
                    return EBTNodeResult::Succeeded;
                }
            }
        }
    }

    return EBTNodeResult::Failed;

}

