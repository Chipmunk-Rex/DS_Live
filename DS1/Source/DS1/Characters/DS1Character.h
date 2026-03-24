// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DS1Character.generated.h"

struct FInputActionValue;

UCLASS()
class DS1_API ADS1Character : public ACharacter
{
	GENERATED_BODY()

public:
	ADS1Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	bool IsMoving() const;

protected:
	UPROPERTY(EditAnywhere, Category = Sprinting)
	float SprintingSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category = Sprinting)
	float NormalSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = Sprinting)
	float UseStamina = 0.1f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

protected:
	/** 캐릭터의 스탯 관리*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDS1AttributeComponent> AttributeComponent;

// UI Section
protected:
	/** WBP_PlayerHUD 에셋 포인터 */
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	/** 생성된 WBP_PlayerHUD 위젯의 포인터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDS1PlayerHUDWidget> PlayerHUDWidget;

#pragma region InputSystem
public:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);

	void Sprinting();
	void StopSprint();

protected:
	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> SprintRollingAction;
#pragma endregion
};
