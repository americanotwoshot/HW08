#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "BlindItem.generated.h"

UCLASS()
class SPARTAPROJECT_API ABlindItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ABlindItem();

protected:
	virtual void ActivateItem(AActor* Activator) override;
};
