// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Weapon.h"
#include "Components/DS1CombatComponent.h"

ADS1Weapon::ADS1Weapon()
{
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
