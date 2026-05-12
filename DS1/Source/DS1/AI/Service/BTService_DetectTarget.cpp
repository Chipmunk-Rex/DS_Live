// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_DetectTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("DetectTarget");
	Interval = 1.0f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(ControllingPawn);

	FVector CenterLocation = ControllingPawn->GetActorLocation();

	FCollisionObjectQueryParams CollisionObjectParam;
	CollisionObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	bool bResult = World->OverlapMultiByObjectType(
		OverlapResults,
		CenterLocation,
		FQuat::Identity,
		CollisionObjectParam,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionParam
	);
	
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* TargetPawn = Cast<APawn>(OverlapResult.GetActor());
			if (TargetPawn && TargetPawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKey.SelectedKeyName, TargetPawn);

				DrawDebugSphere(World, CenterLocation, DetectRadius, 16, FColor::Green, false, 0.2f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKey.SelectedKeyName, nullptr);
	DrawDebugSphere(World, CenterLocation, DetectRadius, 16, FColor::Red, false, 0.2f);
}
