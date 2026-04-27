// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1AttributeComponent.h"
#include "DS1GameplayTags.h"
#include "Components/DS1StateComponent.h"

// Sets default values for this component's properties
UDS1AttributeComponent::UDS1AttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDS1AttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDS1AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDS1AttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UDS1AttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);

	BroadcastAttributeChanged(EDS1AttributeType::Stamina);
}

void UDS1AttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UDS1AttributeComponent::RegenerateStamina, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void UDS1AttributeComponent::RegenerateStamina()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRegenRate, 0.0f, MaxStamina);

	BroadcastAttributeChanged(EDS1AttributeType::Stamina);

	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}

void UDS1AttributeComponent::BroadcastAttributeChanged(EDS1AttributeType InAttributeType) const
{
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.0f;

		switch (InAttributeType)
		{
		case EDS1AttributeType::Stamina:
			Ratio = BaseStamina / MaxStamina;
			break;
		case EDS1AttributeType::Health:
			Ratio = BaseHealth / MaxHealth;
			break;
		default:
			break;
		}

		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}

void UDS1AttributeComponent::TakeDamageAmount(float DamageAmount)
{
	// 체력 차감
	BaseHealth = FMath::Clamp(BaseHealth - DamageAmount, 0.0f, MaxHealth);

	// 체력 차감 알림
	BroadcastAttributeChanged(EDS1AttributeType::Health);

	if (BaseHealth <= 0.0f)
	{
		// 죽음처리
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}

		// State Component -> Set Death State
		UDS1StateComponent* StateComponent = GetOwner()->FindComponentByClass<UDS1StateComponent>();
		if (StateComponent)
		{
			StateComponent->SetCurrentState(DS1GameplayTags::Character_State_Death);
		}
	}
}

