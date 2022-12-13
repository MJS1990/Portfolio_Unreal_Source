#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_Base.generated.h"

UCLASS()
class PORTFOLIO_API UCWidget_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateHP(float Remain, float Max);

private:
	TArray<class UBorder*> Borders;
	TArray<FVector2D> DefaultAlignments;

private:
	float PrevTime = 0.0f;
	class UProgressBar* HealthBar;
};
