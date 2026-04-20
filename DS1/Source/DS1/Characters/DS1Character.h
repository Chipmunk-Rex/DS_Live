// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
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
	bool CanToggleCombat() const;
	bool CanPerformAttack(const FGameplayTag& AttackTag) const;

// Attack Section
public:
	/** 콤보 실행 */
	void ExecuteComboAttack(const FGameplayTag& AttackTag);

	/** 공격 실행 */
	void DoAttack(const FGameplayTag& AttackTag);

	/** 콤보 초기화 */
	void ResetComboAttack();

	/** 콤보 종료*/
	void AttackFinished(const float ComboResetDelay);

	/** 콤보 공격*/
	void EnableComboWindow();
	void DisableComboWindow();

public:
	FORCEINLINE class UDS1AttributeComponent* GetAttributeComponent() const { return AttributeComponent; }

	FORCEINLINE class UDS1StateComponent* GetStateComponent() const { return StateComponent; }

protected:
	UPROPERTY(EditAnywhere, Category = Sprinting)
	float SprintingSpeed = 750.0f;

	UPROPERTY(EditAnywhere, Category = Sprinting)
	float NormalSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = Sprinting)
	float UseStamina = 0.1f;

// Combo Section
protected:
	/**콤보 입력 가능?*/
	bool bCanComboInput = false;

	/** 콤보 입력 여부 */
	bool bSavedComboInput = false;

	/** 콤보 카운트*/
	int32 ComboCount = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

protected:
	/** 애니메이션 관리*/
	UPROPERTY(EditAnywhere, Category = Montage)
	TObjectPtr<UAnimMontage> RollingMontage;

protected:
	/** 캐릭터의 스탯 관리*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDS1AttributeComponent> AttributeComponent;

	/** 캐릭터의 상태 관리*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDS1StateComponent> StateComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDS1CombatComponent> CombatComponent;

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
	void Rolling();
	void Interact();
	void ToggleCombat();

	void LightAttack();
	void SpecialAttack();

protected:
	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> SprintRollingAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> ToggleCombatAction;

	UPROPERTY(EditAnywhere, Category = InputSystem)
	TObjectPtr<class UInputAction> AttackAction;
#pragma endregion
};
