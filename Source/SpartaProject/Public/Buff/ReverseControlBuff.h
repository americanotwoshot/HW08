#pragma once

#include "CoreMinimal.h"
#include "Buff/BuffBase.h"
#include "ReverseControlBuff.generated.h"

UCLASS()
class SPARTAPROJECT_API UReverseControlBuff : public UBuffBase
{
	GENERATED_BODY()

public:
	UReverseControlBuff();
	
	virtual void Activate(ACharacter* Target) override;
	virtual void Deactivate(ACharacter* Target) override;
	
};
