#include "Item/FastItem.h"

#include "Buff/FastBuff.h"
#include "SpartaCharacter.h"

AFastItem::AFastItem()
{
	ItemType = "Fast";
}

void AFastItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator))
		{
			Character->AddBuff(UFastBuff::StaticClass());
		}

		DestroyItem();
	}
}
