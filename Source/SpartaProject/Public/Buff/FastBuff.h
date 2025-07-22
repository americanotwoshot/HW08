#pragma once

#include "CoreMinimal.h"
#include "Buff/BuffBase.h"
#include "FastBuff.generated.h"

UCLASS()
class SPARTAPROJECT_API UFastBuff : public UBuffBase
{
	GENERATED_BODY()

public:
	UFastBuff();
	
	virtual void Activate(ACharacter* Target) override;
	virtual void Deactivate(ACharacter* Target) override;

private:
	float FastAmount;
};
