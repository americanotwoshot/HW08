#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuffContainerWidget.generated.h"

class UHorizontalBox;
class UBuffBase;
class UBuffIconWidget;

UCLASS()
class SPARTAPROJECT_API UBuffContainerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BuffListBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	TSubclassOf<UBuffIconWidget> BuffIconWidgetClass;

	UPROPERTY()
	TMap<UBuffBase*, UBuffIconWidget*> BuffWidgetMap;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Buff")
	void AddBuffIcon(UBuffBase* Buff);

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void RemoveBuffIcon(UBuffBase* Buff);
};
