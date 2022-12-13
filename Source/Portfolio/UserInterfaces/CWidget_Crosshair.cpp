#include "CWidget_Crosshair.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

void UCWidget_Crosshair::UpdateSpreadRange(float InRadius, float InMaxRadius)
{
	Radius = InRadius;
	MaxRadius = InMaxRadius;
}

void UCWidget_Crosshair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* panel = Cast<UCanvasPanel>(WidgetTree->RootWidget);

	TArray<UWidget *> widgets = panel->GetAllChildren();
	for (UWidget* widget : widgets)
	{
		UBorder* border = Cast<UBorder>(widget);
		if (!!border)
		{
			UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);

			DefaultAlignments.Add(slot->GetAlignment());
			Borders.Add(border);
		}
	}
}

void UCWidget_Crosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float minimum = 0;
	float maximum = 0;

	for (int32 i = 0; i < (int32)Direction::Max; i++)
	{
		UBorder* border = Borders[i];
		UCanvasPanelSlot* slot = UWidgetLayoutLibrary::SlotAsCanvasSlot(border);

		switch ((Direction)i)
		{
			case Direction::Top:
			{
				minimum = DefaultAlignments[i].Y;
				maximum = DefaultAlignments[i].Y + MaxRadius;
			}
			break;

			case Direction::Bottom:
			{
				minimum = DefaultAlignments[i].Y;
				maximum = DefaultAlignments[i].Y - MaxRadius;
			}
			break;

			case Direction::Right:
			{
				minimum = DefaultAlignments[i].X;
				maximum = DefaultAlignments[i].X - MaxRadius;
			}
			break;

			case Direction::Left:
			{
				minimum = DefaultAlignments[i].X;
				maximum = DefaultAlignments[i].X + MaxRadius;
			}
			break;
		}

		float spread = FMath::Lerp<float>(minimum, maximum, Radius);

		switch ((Direction)i)
		{
			case Direction::Top:
			case Direction::Bottom:
				slot->SetAlignment(FVector2D(DefaultAlignments[i].X, spread));
				break;

			case Direction::Right:
			case Direction::Left:
				slot->SetAlignment(FVector2D(spread, DefaultAlignments[i].Y));
				break;
		}
	}
}