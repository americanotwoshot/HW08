#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "Buff/Widget/BuffContainerWidget.h"
#include "Buff/BuffBase.h"
#include "Components/Image.h"

ASpartaPlayerController::ASpartaPlayerController()
	: InputMappingContext(nullptr)
	, MoveAction(nullptr)
	, JumpAction(nullptr)
	, LookAction(nullptr)
	, SprintAction(nullptr)
	, HUDWidgetClass(nullptr)
	, HUDWidgetInstance(nullptr)
	, MainMenuWidgetClass(nullptr)
	, MainMenuWidgetInstance(nullptr)
	, GameOverWidgetClass(nullptr)
	, GameOverWidgetInstance(nullptr)
	, BuffContainerWidgetClass(nullptr)
	, BuffContainerWidgetInstance(nullptr)
{
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu();
	}
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ASpartaPlayerController::ShowGameHUD()
{
	ResetUI();

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			if (ASpartaGameState* SpartaGameState = GetWorld()->GetGameState<ASpartaGameState>())
			{
				SpartaGameState->UpdateHUD();
			}
		}
	}

	if (BuffContainerWidgetClass)
	{
		BuffContainerWidgetInstance = CreateWidget<UBuffContainerWidget>(this, BuffContainerWidgetClass);
		if (BuffContainerWidgetInstance)
		{
			BuffContainerWidgetInstance->AddToViewport();
		}
	}
}

void ASpartaPlayerController::ShowMainMenu()
{
	ResetUI();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ASpartaPlayerController::StartGame()
{
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BasicLevel"));
	SetPause(false);
}

void ASpartaPlayerController::ShowGameOverMenu()
{
	ResetUI();

	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance)
		{
			GameOverWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
	
	// if (UFunction* PlayAnimFunc = GameOverWidgetInstance->FindFunction(FName("PlayGameOverAnim")))
	// {
	// 	MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
	// }
	
	if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverWidgetInstance->GetWidgetFromName(TEXT("TotalScoreText"))))
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)));
		}
	}
}

void ASpartaPlayerController::AddBuffIcon(UBuffBase* Buff)
{
	if (BuffContainerWidgetInstance && Buff)
	{
		BuffContainerWidgetInstance->AddBuffIcon(Buff);
	}
}

void ASpartaPlayerController::RemoveBuffIcon(UBuffBase* Buff)
{
	if (BuffContainerWidgetInstance && Buff)
	{
		BuffContainerWidgetInstance->RemoveBuffIcon(Buff);
	}
}

void ASpartaPlayerController::BlindGame(const bool bBlind)
{
	if (HUDWidgetInstance)
	{
		if (UImage* BlindImage = Cast<UImage>(HUDWidgetInstance->GetWidgetFromName(TEXT("BlindImage"))))
		{
			if (bBlind)
			{
				BlindImage->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				BlindImage->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void ASpartaPlayerController::ResetUI()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromParent();
		GameOverWidgetInstance = nullptr;
	}

	if (BuffContainerWidgetInstance)
	{
		BuffContainerWidgetInstance->RemoveFromParent();
		BuffContainerWidgetInstance = nullptr;
	}
}
