// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/DS1Equipment.h"
#include "GameFramework/Character.h"

// Sets default values
ADS1Equipment::ADS1Equipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

// Called when the game starts or when spawned
void ADS1Equipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADS1Equipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

// Called every frame
void ADS1Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADS1Equipment::EquipItem()
{
}

void ADS1Equipment::UnquipItem()
{
}

void ADS1Equipment::AttachToOwner(FName SocketName)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh();
		if (CharacterMesh)
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

