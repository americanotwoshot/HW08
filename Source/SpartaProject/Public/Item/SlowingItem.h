#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "SlowingItem.generated.h"

UCLASS()
class SPARTAPROJECT_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ASlowingItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;
};
