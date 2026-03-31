// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/DS1InteractionInterface.h"
#include "DS1PickupItem.generated.h"

UCLASS()
class DS1_API ADS1PickupItem : public AActor, public IDS1InteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADS1PickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Interact(AActor* InteractActor) override;

public:
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<class ADS1Equipment>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr<class UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<class ADS1Equipment> EquipmentClass;
};
