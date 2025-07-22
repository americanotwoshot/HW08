#pragma once

#include "CoreMinimal.h"
#include "Buff/BuffBase.h"
#include "BlindBuff.generated.h"

UCLASS()
class SPARTAPROJECT_API UBlindBuff : public UBuffBase
{
	GENERATED_BODY()

public:
	UBlindBuff();
	
	virtual void Activate(ACharacter* Target) override;
	virtual void Deactivate(ACharacter* Target) override;
};
