#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "SpartaGameState.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
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

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	MouseSensitivity = 1.0f;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHp();
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
	UpdateOverheadHp();
	
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
	UpdateOverheadHp();
}

void ASpartaCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MoveInput = Value.Get<FVector2D>();
	
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

void ASpartaCharacter::UpdateOverheadHp()
{
	if (!OverheadWidget) return;

	if (UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject())
	{
		if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverheadHP"))))
		{
			HPText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHealth, MaxHealth)));
		}
	}
	
}
