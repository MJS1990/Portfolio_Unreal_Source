#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_Crosshair.generated.h"

UCLASS()
class PORTFOLIO_API UCWidget_Crosshair : public UUserWidget
{
	GENERATED_BODY()
		
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateSpreadRange(float InRadius, float InMaxRadius);

private:
	TArray<class UBorder*> Borders;
	TArray<FVector2D> DefaultAlignments;

private:
	enum class Direction
	{
		Top = 0, Bottom, Right, Left, Max,
	};

private:
	float Radius;
	float MaxRadius;

	float PrevSpread = 0.0f;
	float PrevTime = 0.0f;
};
