// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DS1AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDS1AttributeComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	/** 스태미나가 충분한지 체크*/
	bool CheckHasEnoughStamina(float StaminaCost) const;

	/** 스태미나 차감*/
	void DecreaseStamina(float StaminaCost);

	/** 스태미너 재충전/중지 토글 */
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.0f);

	/** 스태미나 재충전 처리 함수 */
	void RegenerateStamina();

protected:
	UPROPERTY(EditAnywhere, Category = Attribute)
	float BaseStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category = Attribute)
	float StaminaRegenRate = 0.2f;

	FTimerHandle StaminaRegenTimerHandle;
};
