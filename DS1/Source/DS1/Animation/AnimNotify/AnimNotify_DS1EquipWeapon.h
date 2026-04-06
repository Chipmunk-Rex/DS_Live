// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DS1EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UAnimNotify_DS1EquipWeapon : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_DS1EquipWeapon();

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEquipWeapon;
};
