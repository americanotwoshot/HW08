#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffIconWidget.generated.h"

class UImage;
class UTextBlock;
class UBuffBase;

UCLASS()
class SPARTAPROJECT_API UBuffIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BuffName;
	
	void Init(UTexture2D* Texture, const FText& Name);
};
