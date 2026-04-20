// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DS1AttackFinished.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UAnimNotify_DS1AttackFinished : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_DS1AttackFinished();

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.0f;
};
