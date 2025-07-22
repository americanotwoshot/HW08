#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "FastItem.generated.h"

UCLASS()
class SPARTAPROJECT_API AFastItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AFastItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;
};
