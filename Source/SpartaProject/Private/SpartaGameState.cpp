#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"

#include "Item/CoinItem.h"
#include "Item/MineItem.h"
#include "Item/Spawn/SpawnVolume.h"
#include "Blueprint/UserWidget.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

ASpartaGameState::ASpartaGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	CurrentLevelIndex = 0;
	MaxLevelIndex = 3;
	WaveDuration = 20.0f;
	CurrentWaveIndex = 0;
	MaxWaveIndex = 3;
	WaveItemCount.Add(40);
	WaveItemCount.Add(15);
	WaveItemCount.Add(20);
	SpawnedObstacleCount = 5;
	ExplosionCount = 10;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();
	
	StartLevel();

	GetWorldTimerManager().SetTimer(HUDUpdateTimerHandle, this, &ASpartaGameState::UpdateHUD, 0.1f, true);
}

int32 ASpartaGameState::GetScore() const
{
	return Score;
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->AddScore(Amount);
		}
	}

	OnCoinCollected();
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowGameOverMenu();
		}
	}
}

void ASpartaGameState::LoadMenuLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MenuLevel"));
}

void ASpartaGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex + 1)));
				}

				UFunction* PlayAnimFunc = HUDWidget->FindFunction(FName("PlayLevelAnim"));
				HUDWidget->ProcessEvent(PlayAnimFunc, nullptr);
			}
		}
	}

	StartWave();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
		{
			SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	if (CurrentLevelIndex >= MaxLevelIndex)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (URadialSlider* TimeSlider = Cast<URadialSlider>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeSlider->SetValue((WaveDuration - RemainingTime) / WaveDuration);
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance))
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
			}
		}
	}
}

void ASpartaGameState::StartWave()
{
	CurrentWaveIndex++;
	if (CurrentWaveIndex > MaxWaveIndex)
	{
		EndLevel();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Wave %d Start"), CurrentWaveIndex);

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	// 아이템 생성
	for (int32 i = 0; i < WaveItemCount[CurrentWaveIndex - 1]; i++)
	{
		if (!FoundVolumes.IsEmpty())
		{
			if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
			{
				AActor* SpawnedItem = SpawnVolume->SpawnRandomItem();
				if (SpawnedItem && SpawnedItem->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	// Wave 2 : 장애물 생성 
	if (CurrentWaveIndex == 2)
	{
		for (int32 i = 0; i < SpawnedObstacleCount; i++)
		{
			if (!FoundVolumes.IsEmpty())
			{
				if (ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]))
				{
					SpawnVolume->SpawnObstacle();
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("Spike Obstacle Spawned!!"));
	}

	// Wave 3 : 폭발 아이템 발동
	if (CurrentWaveIndex == 3)
	{
		ExplosionCount = 10;
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMineItem::StaticClass(), FoundMines);
		
		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ASpartaGameState::ExplodeMineItem, 1.0f, true);
	}

	// Wave UI 업데이트 및 애니메이션 재생
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave %d"), CurrentWaveIndex)));
				}

				UFunction* PlayAnimFunc = HUDWidget->FindFunction(FName("PlayWaveAnim"));
				HUDWidget->ProcessEvent(PlayAnimFunc, nullptr);
			}
		}
	}

	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &ASpartaGameState::StartWave, WaveDuration, false);
}

void ASpartaGameState::ExplodeMineItem()
{
	ExplosionCount--;
	if (ExplosionCount <= 0)
	{
		GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
	}
	
	if (AMineItem* ActivatingMineItem = Cast<AMineItem>(GetRandomMineItem()))
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->GetPawn())
		{
			ActivatingMineItem->ActivateItem(PC->GetPawn());
		}
	}
}

AActor* ASpartaGameState::GetRandomMineItem()
{
	TArray<AActor*> AvailableMines;

	for (AActor* Actor : FoundMines)
	{
		if (AMineItem* Mine = Cast<AMineItem>(Actor))
		{
			if (!Mine->IsItemActivated())
			{
				AvailableMines.Add(Mine);
			}
		}
	}

	if (AvailableMines.Num() == 0)
	{
		return nullptr;
	}

	const int32 RandomIndex = FMath::RandRange(0, AvailableMines.Num() - 1);
	return AvailableMines[RandomIndex];
}

