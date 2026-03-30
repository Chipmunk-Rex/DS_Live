// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DS1StateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1StateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDS1StateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** 현재 상태 Getter/ Setter */
	FORCEINLINE void SetCurrentState(const FGameplayTag NewState) { CurrentState = NewState; }
	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; }

	/** 현재 상태 초기화 */
	void ClearCurrentState();

	/** 현재 상태 체크하는 함수 [OR ]*/
	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;

public:
	FORCEINLINE bool MovementInputEnabled() { return bMovementInputEnabled; }

	/** 이동 입력 토글 */
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);

	UFUNCTION()
	void MovementInputEnableAction();

protected:
	/** 캐릭터의 현재 상태*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	FGameplayTag CurrentState;

	/** 이동키 입력 상태 관리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
	bool bMovementInputEnabled = true;
};
