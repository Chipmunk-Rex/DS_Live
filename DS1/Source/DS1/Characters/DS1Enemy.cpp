
#include "Characters/DS1Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/DS1AttributeComponent.h"
#include "Components/DS1StateComponent.h"
#include "Components/WidgetComponent.h"
#include "GameplayTagContainer.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "UI/DS1StatBarWidget.h"

ADS1Enemy::ADS1Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Attribute 컴포넌트 생성
	AttributeComponent = CreateDefaultSubobject<UDS1AttributeComponent>(TEXT("Attribute"));

	// OnDeath Delegate 함수 바인딩
	AttributeComponent->OnDeath.AddUObject(this, &ADS1Enemy::OnDeath);

	// OnAttributeChanged 함수 바인딩
	AttributeComponent->OnAttributeChanged.AddUObject(this, &ADS1Enemy::OnAtrributeChanged);

	// State 컴포넌트 생성
	StateComponent = CreateDefaultSubobject<UDS1StateComponent>(TEXT("State"));

	// Health Bar
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	HealthBarWidgetComponent->SetDrawSize(FVector2D(100.0f, 5.0f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void ADS1Enemy::BeginPlay()
{
	Super::BeginPlay();

	SetupHealthBar();
}

// Called every frame
void ADS1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADS1Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ADS1Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ToggleHealthBarVisibility(true);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// 데미지 방향
		FVector ShotDirection = PointDamageEvent->ShotDirection;

		// 히트 위치(표면 접촉 관점)
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;

		// 히트 방향
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;

		// 히트 객체 위치
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);

		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void ADS1Enemy::OnDeath()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	if (CapsuleComp)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Ragdoll
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (MeshComp)
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComp->SetSimulatePhysics(true);
	}

	ToggleHealthBarVisibility(false);
}

void ADS1Enemy::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
}

void ADS1Enemy::HitReaction(const AActor* Attacker)
{
	UAnimMontage* HitReactAnimMontage = GetHitReactAnimation(Attacker);
	if (HitReactAnimMontage)
	{
		float DelaySeconds = PlayAnimMontage(HitReactAnimMontage);
	}
}

UAnimMontage* ADS1Enemy::GetHitReactAnimation(const AActor* Attacker) const
{
	// Enemy가 Attacker를 바라보는 회전 값
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());

	// Enemy의 회전값과 LookAt 회전값의 차이
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);

	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.0f, 45.0f))
	{
		HitDirection = EHitDirection::Front;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.0f, 135.0f))
	{
		HitDirection = EHitDirection::Left;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.0f, 180.0f) || UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.0f, -135.0f))
	{
		HitDirection = EHitDirection::Back;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.0f, -45.0f))
	{
		HitDirection = EHitDirection::Right;
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = HitReactAnimFront;
		break;
	case EHitDirection::Back:
		SelectedMontage = HitReactAnimBack;
		break;
	case EHitDirection::Left:
		SelectedMontage = HitReactAnimLeft;
		break;
	case EHitDirection::Right:
		SelectedMontage = HitReactAnimRight;
		break;
	default:
		break;
	}

	return SelectedMontage;
}

void ADS1Enemy::ToggleHealthBarVisibility(bool bVisibility)
{
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(bVisibility);
	}
}

void ADS1Enemy::SetupHealthBar()
{
	if (HealthBarWidgetComponent)
	{
		UDS1StatBarWidget* StatBar = Cast<UDS1StatBarWidget>(HealthBarWidgetComponent->GetWidget());
		if (StatBar)
		{
			StatBar->SetStatBarColor(FLinearColor::Red);
		}
	}

	if (AttributeComponent)
	{
		AttributeComponent->BroadcastAttributeChanged(EDS1AttributeType::Health);
	}
}

void ADS1Enemy::OnAtrributeChanged(EDS1AttributeType AttributeType, float InValue)
{
	if (AttributeType == EDS1AttributeType::Health)
	{
		if (HealthBarWidgetComponent)
		{
			const UDS1StatBarWidget* StatBar = Cast<UDS1StatBarWidget>(HealthBarWidgetComponent->GetWidget());
			if (StatBar)
			{
				StatBar->SetRatio(InValue);
			}
		}
	}
}

