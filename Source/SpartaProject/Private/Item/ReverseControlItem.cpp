#include "Item/ReverseControlItem.h"

#include "SpartaCharacter.h"
#include "Buff/ReverseControlBuff.h"

AReverseControlItem::AReverseControlItem()
{
	ItemType = "ReverseControl";
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* Character = Cast<ASpartaCharacter>(Activator))
		{
			Character->AddBuff(UReverseControlBuff::StaticClass());
		}

		DestroyItem();
	}
}
