#include "Item/CoinItem.h"

#include "SpartaGameState.h"
#include "Engine/World.h"

ACoinItem::ACoinItem()
{
	ItemType = "DefaultCoin";
	PointValue = 0;
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				GameState->AddScore(PointValue);
			}
		}
		
		DestroyItem();
	}
}
