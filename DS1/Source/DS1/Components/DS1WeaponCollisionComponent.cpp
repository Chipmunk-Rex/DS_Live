// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DS1WeaponCollisionComponent.h"

// Sets default values for this component's properties
UDS1WeaponCollisionComponent::UDS1WeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TraceStartSocketName = FName("WeaponStart");
	TraceEndSocketName = FName("WeaponEnd");
}


// Called when the game starts
void UDS1WeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDS1WeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void UDS1WeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMesh = MeshComponent;
}

void UDS1WeaponCollisionComponent::AddIgnoreActor(AActor* Actor)
{
	IgnoreActors.Add(Actor);
}

void UDS1WeaponCollisionComponent::RemoveIgnoreActor(AActor* Actor)
{
	IgnoreActors.Remove(Actor);
}

bool UDS1WeaponCollisionComponent::CanHitActor(AActor* Actor) const
{
	return AlreadyHitActors.Contains(Actor) == false;
}

void UDS1WeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;

	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoreActors,
		DrawDebugType,
		OutHits,
		true);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor == nullptr)
				continue;

			if (CanHitActor(HitActor))
			{
				AlreadyHitActors.Add(HitActor);

				if (OnHitActor.IsBound())
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}

void UDS1WeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UDS1WeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

