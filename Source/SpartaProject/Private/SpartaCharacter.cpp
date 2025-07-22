#include "SpartaCharacter.h"
#include "SpartaGameState.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Buff/BuffBase.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	InitialSpeed = 600.0f;
	NormalSpeed = InitialSpeed;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	MouseSensitivity = 1.0f;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;

	bIsReverseControl = false;
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateHpGauge();
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
			}

			if (PlayerController->JumpAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartJump);
				EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopJump);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
			}

			if (PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASpartaCharacter::StartSprint);
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
			}
		}
	}
}

float ASpartaCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0, MaxHealth);

	UpdateHpGauge();
	
	if (CurrentHealth <= 0)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

int32 ASpartaCharacter::GetHealth() const
{
	return CurrentHealth;
}

void ASpartaCharacter::AddHealth(int32 Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
	UpdateHpGauge();
}

void ASpartaCharacter::AddBuff(TSubclassOf<UBuffBase> BuffClass)
{
	if (!BuffClass) return;
	
	if (UBuffBase* ExistingBuff = ActiveBuffs.FindRef(BuffClass))
	{
		GetWorldTimerManager().ClearTimer(ExistingBuff->ExpireTimerHandle);
		ExistingBuff->StartTimer(this);
	}
	else
	{
		UBuffBase* NewBuff = NewObject<UBuffBase>(this, BuffClass);
		NewBuff->Activate(this);
		NewBuff->StartTimer(this);
		ActiveBuffs.Add(BuffClass, NewBuff);

		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			SpartaPlayerController->AddBuffIcon(NewBuff);
		}
	}
}

void ASpartaCharacter::RemoveBuff(TSubclassOf<UBuffBase> BuffClass)
{
	if (UBuffBase* ExistingBuff = ActiveBuffs.FindRef(BuffClass))
	{
		GetWorldTimerManager().ClearTimer(ExistingBuff->ExpireTimerHandle);
		ActiveBuffs.Remove(BuffClass);

		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			SpartaPlayerController->RemoveBuffIcon(ExistingBuff);
		}
	}
}

void ASpartaCharacter::SetSpeed(float SpeedMultiplier)
{
	NormalSpeed *= SpeedMultiplier;
	SprintSpeed *= SpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void ASpartaCharacter::ResetSpeed(float SpeedMultiplier)
{
	NormalSpeed /= SpeedMultiplier;
	SprintSpeed /= SpeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void ASpartaCharacter::SetReverseControl(const bool bReverse)
{
	bIsReverseControl = bReverse;
}

void ASpartaCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	FVector2D MoveInput = Value.Get<FVector2D>();

	if (bIsReverseControl)
	{
		MoveInput.X *= -1.0f;
		MoveInput.Y *= -1.0f;
	}
	
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X * MouseSensitivity);
	AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
	}
}

void ASpartaCharacter::UpdateHpGauge()
{
	if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
	{
		if (UUserWidget* HUDWidgetInstance = PlayerController->GetHUDWidget())
		{
			if (UProgressBar* HpGaugeBar = Cast<UProgressBar>(HUDWidgetInstance->GetWidgetFromName(TEXT("HpGauge"))))
			{
				HpGaugeBar->SetPercent((float)CurrentHealth / MaxHealth);
			}
		}
	}
}
