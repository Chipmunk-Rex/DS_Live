// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DS1Equipment.generated.h"

UCLASS()
class DS1_API ADS1Equipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ADS1Equipment();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE UStaticMesh* GetMeshAsset() { return MeshAsset; }

public:
	virtual void EquipItem();
	virtual void UnquipItem();
	virtual void AttachToOwner(FName SocketName);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Equipment | Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	TObjectPtr<UStaticMesh> MeshAsset;
};
