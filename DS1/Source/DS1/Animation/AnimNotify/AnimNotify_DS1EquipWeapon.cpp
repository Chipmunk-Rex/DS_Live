// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_DS1EquipWeapon.h"
#include "Components/DS1CombatComponent.h"
#include "Equipments/DS1Weapon.h"

UAnimNotify_DS1EquipWeapon::UAnimNotify_DS1EquipWeapon()
{
}

void UAnimNotify_DS1EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const AActor* Owner = MeshComp->GetOwner();
	if (Owner)
	{
		UDS1CombatComponent* CombatComponent = Owner->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent)
		{
			ADS1Weapon* MainWeapon = CombatComponent->GetMainWeapon();
			if (MainWeapon)
			{
				bool bCombatEnabled = CombatComponent->IsCombatEnabled();
				FName WeaponSocketName;

				if (bEquipWeapon)
				{
					bCombatEnabled = true;
					WeaponSocketName = MainWeapon->GetEquipSocketName();
				}
				else
				{
					bCombatEnabled = false;
					WeaponSocketName = MainWeapon->GetUnequipSocketName();
				}

				CombatComponent->SetCombatEnabled(bCombatEnabled);
				MainWeapon->AttachToOwner(WeaponSocketName);
			}
		}
	}
}
