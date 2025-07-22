#include "Buff/Widget/BuffIconWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UBuffIconWidget::Init(UTexture2D* Texture, const FText& Name)
{
	if (IconImage && Texture)
	{
		IconImage->SetBrushFromTexture(Texture);
	}

	if (BuffName)
	{
		BuffName->SetText(Name);
	}
}
