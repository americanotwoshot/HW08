#include "Buff/BuffBase.h"
#include "SpartaCharacter.h"
#include "GameFramework/Character.h"

void UBuffBase::Activate(ACharacter* Target)
{
}

void UBuffBase::Deactivate(ACharacter* Target)
{
}

void UBuffBase::StartTimer(ACharacter* Target)
{
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->GetWorld()->GetTimerManager().SetTimer(
			ExpireTimerHandle,
			FTimerDelegate::CreateUObject(this, &UBuffBase::Deactivate, Target),
			Duration,
			false
		);
	}
}
