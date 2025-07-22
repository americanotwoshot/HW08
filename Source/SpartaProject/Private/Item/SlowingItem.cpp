#include "Item/SlowingItem.h"

#include "Buff/SlowingBuff.h"
#include "SpartaCharacter.h"

ASlowingItem::ASlowingItem()
{
	ItemType = "Slowing";
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator))
		{
			Character->AddBuff(USlowingBuff::StaticClass());
		}

		DestroyItem();
	}
}
