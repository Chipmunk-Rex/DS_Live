// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DS1PickupItem.h"
#include "DS1Define.h"
#include "Equipments/DS1Equipment.h"

// Sets default values
ADS1PickupItem::ADS1PickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupItemMesh"));
	SetRootComponent(ItemMesh);

	ItemMesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	ItemMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ADS1PickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADS1PickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (ADS1Equipment* CDO = EquipmentClass->GetDefaultObject<ADS1Equipment>())
		{
			ItemMesh->SetStaticMesh(CDO->GetMeshAsset());
			ItemMesh->SetSimulatePhysics(true);
		}
	}
}

// Called every frame
void ADS1PickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADS1PickupItem::Interact(AActor* InteractActor)
{
	GEngine->AddOnScreenDebugMessage(4, 1.5f, FColor::Blue, TEXT("Interaction PickupItem"));
}
