#include "Item/BlindItem.h"

#include "SpartaCharacter.h"
#include "Buff/BlindBuff.h"

ABlindItem::ABlindItem()
{
	ItemType = "Blind";
}

void ABlindItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator))
		{
			Character->AddBuff(UBlindBuff::StaticClass());
		}

		DestroyItem();
	}
}
