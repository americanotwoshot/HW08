#include "Buff/BlindBuff.h"

#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"

UBlindBuff::UBlindBuff()
{
	ConstructorHelpers::FObjectFinder<UTexture2D> IconTex(TEXT("/Game/Resources/AI/Blind.Blind"));
	if (IconTex.Succeeded())
	{
		BuffIcon = IconTex.Object;
	}
	
	BuffName = FText::FromString(TEXT("Blind"));
}

void UBlindBuff::Activate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("BlindBuff Activated"));

	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(SpartaCharacter->GetController()))
		{
			SpartaPlayerController->BlindGame(true);
		}
	}
}

void UBlindBuff::Deactivate(ACharacter* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("BlindBuff Deactivated"));

	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Target))
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(SpartaCharacter->GetController()))
		{
			SpartaPlayerController->BlindGame(false);
		}
		SpartaCharacter->RemoveBuff(UBlindBuff::StaticClass());
	}
}
