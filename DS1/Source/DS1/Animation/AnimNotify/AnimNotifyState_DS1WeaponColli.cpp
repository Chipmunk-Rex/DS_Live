// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotifyState_DS1WeaponColli.h"
#include "AnimNotifyState_DS1WeaponColli.h"
#include "Components/DS1CombatComponent.h"
#include "Equipments/DS1Weapon.h"

UAnimNotifyState_DS1WeaponColli::UAnimNotifyState_DS1WeaponColli()
{
}

void UAnimNotifyState_DS1WeaponColli::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	const AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor)
	{
		const UDS1CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent)
		{
			ADS1Weapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->TurnOnWeaponCollision();
			}
		}
	}
}

void UAnimNotifyState_DS1WeaponColli::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	const AActor* OwnerActor = MeshComp->GetOwner();
	if (OwnerActor)
	{
		const UDS1CombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UDS1CombatComponent>();
		if (CombatComponent)
		{
			ADS1Weapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->TurnOffWeaponCollision();
			}
		}
	}
}
