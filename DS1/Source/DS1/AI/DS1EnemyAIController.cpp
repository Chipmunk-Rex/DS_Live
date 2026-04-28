// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DS1EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ADS1EnemyAIController::ADS1EnemyAIController()
{
}

void ADS1EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void ADS1EnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();

}

void ADS1EnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool Result = RunBehaviorTree(BTAsset);
		ensure(Result);
	}
}

void ADS1EnemyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}
