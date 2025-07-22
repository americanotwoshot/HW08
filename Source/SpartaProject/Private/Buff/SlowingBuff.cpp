#include "Buff/SlowingBuff.h"

#include "SpartaCharacter.h"

USlowingBuff::USlowingBuff()
	: SlowingAmount(0.5f)
{
	ConstructorHelpers::FObjectFinder<UTexture2D> IconTex(TEXT("/Game/Resources/AI/Slow.Slow"));
	if (IconTex.Succeeded())
	{
		BuffIcon = IconTex.Object;
	}
	
	BuffName = FText::FromString(TEXT("Slow"));
}

void USlowingBuff::Activate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("SlowingBuff Activated"));
	
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->SetSpeed(SlowingAmount);
	}
}

void USlowingBuff::Deactivate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("SlowingBuff Deactivated"));
	
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->ResetSpeed(SlowingAmount);
		SpartaCharacter->RemoveBuff(USlowingBuff::StaticClass());
	}
}
