// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Weapon.h"
#include "Components/DS1CombatComponent.h"
#include "Data/DS1MontageActionData.h"
#include "DS1GameplayTags.h"

ADS1Weapon::ADS1Weapon()
{
	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Light, 3.0f);
	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Special, 15.0f);
}

void ADS1Weapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<UDS1CombatComponent>();
	if (CombatComponent)
	{
		CombatComponent->SetMainWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;

		AttachToOwner(AttachSocket);
	}
}

UAnimMontage* ADS1Weapon::GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index)
{
	if (MontageActionData)
	{
		return MontageActionData->GetMontageGroupForTag(GroupTag, Index);
	}

	return nullptr;
}

float ADS1Weapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	return 0.f;
}
