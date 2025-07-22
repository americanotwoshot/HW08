#include "Buff/FastBuff.h"
#include "SpartaCharacter.h"


UFastBuff::UFastBuff()
	: FastAmount(2.0f)
{
	ConstructorHelpers::FObjectFinder<UTexture2D> IconTex(TEXT("/Game/Resources/AI/Fast.Fast"));
	if (IconTex.Succeeded())
	{
		BuffIcon = IconTex.Object;
	}
	
	BuffName = FText::FromString(TEXT("Fast"));
}

void UFastBuff::Activate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("FastBuff Activated"));
	
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->SetSpeed(FastAmount);
	}
}

void UFastBuff::Deactivate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("FastBuff Deactivated"));
	
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->ResetSpeed(FastAmount);
		SpartaCharacter->RemoveBuff(UFastBuff::StaticClass());
	}
}
