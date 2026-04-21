// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DS1WeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DS1_API UDS1WeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDS1WeaponCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);
	void AddIgnoreActor(AActor* Actor);
	void RemoveIgnoreActor(AActor* Actor);

public:
	bool CanHitActor(AActor* Actor) const;

	void CollisionTrace();

	void TurnOnCollision();
	void TurnOffCollision();

public:
	// 히트시 호출되는 델리게이트
	FOnHitActor OnHitActor;

protected:
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;

	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;

protected:
	// 무기의 Mesh Component
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> WeaponMesh;
		
	// 충돌이 발생한 액터 예외처리용 배열
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	// 충돌 체크 활성화 여부
	bool bIsCollisionEnabled = false;

protected:
	// Sphere 반지름
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.0f;

	// 충돌 대상 오브젝트 타입
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	// 충돌 제외 액터들
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoreActors;

	// 디버그 드로우 타입
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;
};
