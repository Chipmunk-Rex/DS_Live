// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DS1Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/DS1AttributeComponent.h"
#include "UI/DS1PlayerHUDWidget.h"

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

void ADS1Character::Input_Move(const FInputActionValue& InputValue)
{
	if (bMovementInputEnabled == false)
		return;

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

	if (AttributeComponent->CheckHasEnoughStamina(15.0f))
	{
		// 이동입력 처리 무시
		bMovementInputEnabled = false;

		// 스태미나 충전 멈춤
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 스태미나 차감
		AttributeComponent->DecreaseStamina(15.0f);

		// 롤링 애니메이션 재생
		PlayAnimMontage(RollingMontage);

		// 스태미나 충전 시작
		AttributeComponent->ToggleStaminaRegeneration(true);
	}
}

