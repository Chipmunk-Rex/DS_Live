// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS1Define.h"
#include "GameFramework/Character.h"
#include "Interfaces/DS1AICombatInterface.h"
#include "DS1Enemy.generated.h"

struct FGameplayTag;
class ATargetPoint;

UCLASS()
class DS1_API ADS1Enemy : public ACharacter, public IDS1AICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADS1Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void PerformAttack(FGameplayTag& AttakTag, FOnMontageEnded& MontageEnedDelegate) override;

public:
	virtual void OnDeath();

protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;

public:
	/** 체력바 토글 */ 
	void ToggleHealthBarVisibility(bool bVisibility);

	/** 체력바 설정*/
	void SetupHealthBar();

	/** Attribute 변경 델리게이트 함수 */
	void OnAtrributeChanged(EDS1AttributeType AttributeType, float InValue);

public:
	FORCEINLINE ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}

	FORCEINLINE void IncrementPatrolIndex()
	{
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}

// AI Section
protected:
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<TObjectPtr<ATargetPoint>> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;

// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystem> ImpactParticle;

// Montage Section
protected:
	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	TObjectPtr<UAnimMontage> HitReactAnimRight;

// 기본 무기 설정
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADS1Weapon> DefaultWeaponClass;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDS1AttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDS1StateComponent> StateComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDS1CombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UWidgetComponent> HealthBarWidgetComponent;
};
