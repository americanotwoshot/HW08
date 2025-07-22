#pragma once

#include "CoreMinimal.h"
#include "BuffBase.generated.h"

class UBuffIconWidget;

UCLASS()
class SPARTAPROJECT_API UBuffBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff")
	UTexture2D* BuffIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff")
	FText BuffName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float Duration = 5.0f;
	
	UPROPERTY()
	FTimerHandle ExpireTimerHandle;

	virtual void Activate(ACharacter* Target);
	virtual void Deactivate(ACharacter* Target);

	void StartTimer(ACharacter* Target);
};
