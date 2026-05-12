// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_PerformAttack.h"
#include "AIController.h"
#include "Components/DS1StateComponent.h"
#include "Interfaces/DS1AICombatInterface.h"

UBTTask_PerformAttack::UBTTask_PerformAttack()
{
	NodeName = TEXT("PerformAttack");
}

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IDS1AICombatInterface* CombatInterface = Cast<IDS1AICombatInterface>(ControllingPawn);
	if (CombatInterface)
	{
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindLambda(
			[this, &OwnerComp, ControllingPawn](UAnimMontage* Montage, bool bInterrupted)
			{
				if (::IsValid(&OwnerComp) == false)
				{
					EBTNodeResult::Failed;
				}

				UDS1StateComponent* StateComponent = ControllingPawn->GetComponentByClass<UDS1StateComponent>();
				if (StateComponent)
				{
					StateComponent->ClearCurrentState();
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});
		// Attack ½ÇÇà
		CombatInterface->PerformAttack(AttackTag, MontageEndedDelegate);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}
