#pragma once

#include "CoreMinimal.h"
#include "Buff/BuffBase.h"
#include "SlowingBuff.generated.h"

UCLASS()
class SPARTAPROJECT_API USlowingBuff : public UBuffBase
{
	GENERATED_BODY()

public:
	USlowingBuff();
	
	virtual void Activate(ACharacter* Target) override;
	virtual void Deactivate(ACharacter* Target) override;

private:
	float SlowingAmount;
};
