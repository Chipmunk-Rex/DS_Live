// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DS1CombatComponent.generated.h"

class ADS1Weapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDS1CombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE ADS1Weapon* GetMainWeapon() const { return MainWeapon; }

	void SetMainWeapon(ADS1Weapon* NewWeapon);

	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE void SetCombatEnabled(bool bEnabled) { bCombatEnabled = bEnabled; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADS1Weapon> MainWeapon;

	/** 전투 활성화 상태*/
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;
};
