// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Weapon.h"
#include "Components/DS1CombatComponent.h"
#include "Data/DS1MontageActionData.h"
#include "DS1GameplayTags.h"
#include "Components/DS1WeaponCollisionComponent.h"
#include "Kismet/GameplayStatics.h"

ADS1Weapon::ADS1Weapon()
{
	WeaponCollisionComponent = CreateDefaultSubobject<UDS1WeaponCollisionComponent>(TEXT("WeaponCollision"));

	WeaponCollisionComponent->OnHitActor.AddUObject(this, &ADS1Weapon::OnHitActor);

	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Light, 3.0f);
	StaminaCostMap.Add(DS1GameplayTags::Character_Attack_Special, 15.0f);

	DamageMultiplierMap.Add(DS1GameplayTags::Character_Attack_Light, 1.0f);
	DamageMultiplierMap.Add(DS1GameplayTags::Character_Attack_Special, 2.0f);
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

		if (WeaponCollisionComponent)
		{
			// 무기 메쉬 정보
			WeaponCollisionComponent->SetWeaponMesh(Mesh);
			
			// 무기를 소유한 Actor를 Ignore
			WeaponCollisionComponent->AddIgnoreActor(GetOwner());
		}
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

float ADS1Weapon::GetAttackDamage(FGameplayTag InAttackType) const
{
	const AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		const FGameplayTag LastAttackTag = (InAttackType == FGameplayTag::EmptyTag) ? GetLastAttackType() : InAttackType;

		if (DamageMultiplierMap.Contains(LastAttackTag))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackTag];
			return BaseDamage * Multiplier;
		}
	}

	return BaseDamage;
}

void ADS1Weapon::TurnOnWeaponCollision()
{
	if (WeaponCollisionComponent)
	{
		WeaponCollisionComponent->TurnOnCollision();
	}
}

void ADS1Weapon::TurnOffWeaponCollision()
{
	if (WeaponCollisionComponent)
	{
		WeaponCollisionComponent->TurnOffCollision();
	}
}

void ADS1Weapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();

	FVector DamageDirection = GetOwner()->GetActorForwardVector();

	float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor,
		AttackDamage,
		DamageDirection,
		Hit,
		GetOwner()->GetInstigatorController(),
		this,
		nullptr);
}
