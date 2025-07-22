#include "Buff/ReverseControlBuff.h"

#include "SpartaCharacter.h"

UReverseControlBuff::UReverseControlBuff()
{
	ConstructorHelpers::FObjectFinder<UTexture2D> IconTex(TEXT("/Game/Resources/AI/ReverseControlImage.ReverseControlImage"));
	if (IconTex.Succeeded())
	{
		BuffIcon = IconTex.Object;
	}
	
	BuffName = FText::FromString(TEXT("Reverse"));
}

void UReverseControlBuff::Activate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("ReverseControlBuff Activated"));

	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->SetReverseControl(true);
	}
}

void UReverseControlBuff::Deactivate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("ReverseControlBuff Deactivated"));
	
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		SpartaCharacter->SetReverseControl(false);
		SpartaCharacter->RemoveBuff(UReverseControlBuff::StaticClass());
	}
}
