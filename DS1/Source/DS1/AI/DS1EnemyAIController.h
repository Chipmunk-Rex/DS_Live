// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DS1EnemyAIController.generated.h"

struct FAIStimulus;

/**
 * 
 */
UCLASS()
class DS1_API ADS1EnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADS1EnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void RunAI();
	void StopAI();

protected:
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void SetTarget(AActor* NewTarget);

protected:
	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<class UBehaviorTree> BTAsset;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

	// 시각 감지 타이머
	FTimerHandle SightTimerHandle;
};
