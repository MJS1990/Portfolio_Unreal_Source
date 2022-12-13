#include "UserInterfaces/CWidget_Base.h"
#include "Global.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"

void UCWidget_Base::UpdateHP(float Remain, float Max)
{
	HealthBar->SetPercent(Remain / Max);
}

void UCWidget_Base::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HealthBar = WidgetTree->FindWidget<UProgressBar>("HPBar");
}

void UCWidget_Base::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
