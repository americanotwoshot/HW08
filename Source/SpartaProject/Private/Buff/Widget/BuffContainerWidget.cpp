#include "Buff/Widget/BuffContainerWidget.h"
#include "Buff/BuffBase.h"
#include "Buff/Widget/BuffIconWidget.h"
#include "Components/HorizontalBox.h"

void UBuffContainerWidget::AddBuffIcon(UBuffBase* Buff)
{
	if (!BuffListBox || !BuffIconWidgetClass || !Buff) return;

	UE_LOG(LogTemp, Warning, TEXT("Add Buff Icon"));
	
	if (UBuffIconWidget* NewIcon = CreateWidget<UBuffIconWidget>(GetWorld(), BuffIconWidgetClass))
	{
		NewIcon->Init(Buff->BuffIcon, Buff->BuffName);
		BuffListBox->AddChild(NewIcon);
		BuffWidgetMap.Add(Buff, NewIcon);
	}
}

void UBuffContainerWidget::RemoveBuffIcon(UBuffBase* Buff)
{
	if (!BuffListBox || !BuffIconWidgetClass || !Buff) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Remove Buff Icon"));
	
	UBuffIconWidget* IconWidget = BuffWidgetMap.FindRef(Buff);
	if (IconWidget)
	{
		BuffListBox->RemoveChild(IconWidget);
		IconWidget->RemoveFromParent();
	}

	BuffWidgetMap.Remove(Buff);
}
