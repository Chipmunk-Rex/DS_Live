// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_FindPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/DS1Enemy.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControlledPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ADS1Enemy* Enemy = Cast<ADS1Enemy>(ControlledPawn);
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (Enemy->GetPatrolPoint())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey.SelectedKeyName, Enemy->GetPatrolPoint()->GetActorLocation());

		Enemy->IncrementPatrolIndex();

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
