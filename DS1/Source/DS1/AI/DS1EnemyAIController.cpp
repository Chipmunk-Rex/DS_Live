// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DS1EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Characters/DS1Character.h"

ADS1EnemyAIController::ADS1EnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADS1EnemyAIController::TargetPerceptionUpdated);
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

void ADS1EnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (false == Actor->IsA<ADS1Character>())
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		SetTarget(Actor);

		GetWorld()->GetTimerManager().ClearTimer(SightTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			SightTimerHandle,
			FTimerDelegate::CreateLambda(
				[this]()
				{
					SetTarget(nullptr);
				}
			),
			3.0f,
			false);
	}
}

void ADS1EnemyAIController::SetTarget(AActor* NewTarget)
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("Target"), NewTarget);
	}
}
