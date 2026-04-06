// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DS1AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API UDS1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDS1AnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetCurrentState();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= References)
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = References)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = MovementData)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = MovementData)
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = MovementData)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly, Category = MovementData)
	bool bIsFalling = false;
};
