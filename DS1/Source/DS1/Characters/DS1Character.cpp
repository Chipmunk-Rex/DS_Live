// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DS1Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DS1AttributeComponent.h"
#include "Components/DS1StateComponent.h"
#include "Components/DS1CombatComponent.h"
#include "UI/DS1PlayerHUDWidget.h"
#include "DS1GameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/DS1InteractionInterface.h"
#include "Equipments/DS1Weapon.h"

// Sets default values
ADS1Character::ADS1Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-30, 0, 0));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Character Movement
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	// Chatacter Attribute
	AttributeComponent = CreateDefaultSubobject<UDS1AttributeComponent>(TEXT("Attribute"));

	// Character State
	StateComponent = CreateDefaultSubobject<UDS1StateComponent>(TEXT("State"));

	// Combat
	CombatComponent = CreateDefaultSubobject<UDS1CombatComponent>(TEXT("Combat"));
}

// Called when the game starts or when spawned
void ADS1Character::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UDS1PlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);

		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
}

// Called every frame
void ADS1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed));
}

void ADS1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// IMC_Default를 나의 입력 매핑으로 설정
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		
		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ADS1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADS1Character::Input_Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADS1Character::Input_Look);

		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ADS1Character::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ADS1Character::StopSprint);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ADS1Character::Rolling);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADS1Character::Interact);

		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Triggered, this, &ADS1Character::ToggleCombat);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ADS1Character::LightAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADS1Character::SpecialAttack);
	}
}

bool ADS1Character::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		return (MovementComponent->Velocity.Size2D() > 3.0f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);
	}

	return false;
}

bool ADS1Character::CanToggleCombat() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(DS1GameplayTags::Character_State_Attacking);
	CheckTags.AddTag(DS1GameplayTags::Character_State_Rolling);
	CheckTags.AddTag(DS1GameplayTags::Character_State_GeneralAction);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

bool ADS1Character::CanPerformAttack(const FGameplayTag& AttackTag) const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(DS1GameplayTags::Character_State_Rolling);
	CheckTags.AddTag(DS1GameplayTags::Character_State_GeneralAction);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTag);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false
		&& CombatComponent->IsCombatEnabled()
		&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}

void ADS1Character::ExecuteComboAttack(const FGameplayTag& AttackTag)
{
	check(StateComponent);

	if (StateComponent->GetCurrentState() != DS1GameplayTags::Character_State_Attacking)
	{
		ResetComboAttack();

		DoAttack(AttackTag);
	}
	else if (bCanComboInput)
	{
		bSavedComboInput = true;
	}
}

void ADS1Character::DoAttack(const FGameplayTag& AttackTag)
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	ADS1Weapon* Weapon = CombatComponent->GetMainWeapon();
	if (Weapon)
	{
		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTag, ComboCount);
		if (Montage == nullptr)
		{
			ComboCount = 0;
			Montage = Weapon->GetMontageForTag(AttackTag, ComboCount);
		}

		PlayAnimMontage(Montage);

		StateComponent->SetCurrentState(DS1GameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);

		AttributeComponent->ToggleStaminaRegeneration(false);

		const float StaminaCost = Weapon->GetStaminaCost(AttackTag);
		AttributeComponent->DecreaseStamina(StaminaCost);

		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ADS1Character::ResetComboAttack()
{
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCount = 0;
}

void ADS1Character::AttackFinished(const float ComboResetDelay)
{
	GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Magenta, TEXT("AttackFinished"));

	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
}

void ADS1Character::EnableComboWindow()
{
	bCanComboInput = true;
}

void ADS1Character::DisableComboWindow()
{
	bCanComboInput = false;

	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCount++;
		DoAttack(DS1GameplayTags::Character_Attack_Light);
	}
}

void ADS1Character::Input_Move(const FInputActionValue& InputValue)
{
	check(StateComponent);

	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}

	FVector2D MovementVector = InputValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, MovementVector.X);
		AddMovementInput(RightVector, MovementVector.Y);
	}
}

void ADS1Character::Input_Look(const FInputActionValue& InputValue)
{
	FVector2D LookVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ADS1Character::Sprinting()
{
	if (AttributeComponent)
	{
		if (AttributeComponent->CheckHasEnoughStamina(5.0f) && IsMoving())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

			AttributeComponent->ToggleStaminaRegeneration(false);
			AttributeComponent->DecreaseStamina(UseStamina);
		}
		else
		{
			StopSprint();
		}
	}
}

void ADS1Character::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	if (AttributeComponent)
	{
		AttributeComponent->ToggleStaminaRegeneration(true);
	}
}

void ADS1Character::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);

	if (AttributeComponent->CheckHasEnoughStamina(15.0f))
	{
		// 이동입력 처리 무시
		StateComponent->ToggleMovementInput(false);

		// 스태미나 충전 멈춤
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 스태미나 차감
		AttributeComponent->DecreaseStamina(15.0f);

		// 롤링 애니메이션 재생
		PlayAnimMontage(RollingMontage);

		// 롤링 상태 설정
		StateComponent->SetCurrentState(DS1GameplayTags::Character_State_Rolling);

		// 스태미나 충전 시작
		AttributeComponent->ToggleStaminaRegeneration(true);
	}
}

void ADS1Character::Interact()
{
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	const float Radius = 100.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			IDS1InteractionInterface* Interaction = Cast<IDS1InteractionInterface>(HitActor);
			if (Interaction)
			{
				Interaction->Interact(this);
			}
		}
	}
}

void ADS1Character::ToggleCombat()
{
	check(StateComponent);
	check(CombatComponent);

	if (CombatComponent)
	{
		const ADS1Weapon* Weapon = CombatComponent->GetMainWeapon();
		if (Weapon)
		{
			if (CanToggleCombat())
			{
				StateComponent->SetCurrentState(DS1GameplayTags::Character_State_GeneralAction);

				if (CombatComponent->IsCombatEnabled())
				{
					PlayAnimMontage(Weapon->GetUnequipMontage());
				}
				else
				{
					PlayAnimMontage(Weapon->GetEquipMontage());
				}
			}
		}
	}
}

void ADS1Character::LightAttack()
{
	const FGameplayTag AttackTag = DS1GameplayTags::Character_Attack_Light;

	if (CanPerformAttack(AttackTag))
	{
		GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Magenta, TEXT("LightAttack"));

		ExecuteComboAttack(AttackTag);
	}
}

void ADS1Character::SpecialAttack()
{
	const FGameplayTag AttackTag = DS1GameplayTags::Character_Attack_Special;

	if (CanPerformAttack(AttackTag))
	{
		GEngine->AddOnScreenDebugMessage(4, 1.0f, FColor::Magenta, TEXT("SpecialAttack"));

		ExecuteComboAttack(AttackTag);
	}
}