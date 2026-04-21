// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/DS1Equipment.h"
#include "GameplayTagContainer.h"
#include "DS1Weapon.generated.h"

/**
 * 
 */
UCLASS()
class DS1_API ADS1Weapon : public ADS1Equipment
{
	GENERATED_BODY()
	
public:
	ADS1Weapon();

public:
	virtual void EquipItem() override;

public:
	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }

	FORCEINLINE UAnimMontage* GetEquipMontage() const {	return EquipMontageAnimation; }
	FORCEINLINE UAnimMontage* GetUnequipMontage() const { return UnequipMontageAnimation; }

public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index = 0);

	float GetStaminaCost(const FGameplayTag& InTag) const;

public:
	void TurnOnWeaponCollision();
	void TurnOffWeaponCollision();

	// 무기의 충돌에 검출된 Actor에 데미지 전달
	void OnHitActor(const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	TObjectPtr<class UAnimMontage> EquipMontageAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	TObjectPtr<class UAnimMontage> UnequipMontageAnimation;

// Data Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Montage")
	TObjectPtr<class UDS1MontageActionData> MontageActionData;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

protected:
	UPROPERTY()
	TObjectPtr<class UDS1CombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDS1WeaponCollisionComponent> WeaponCollisionComponent;
};
