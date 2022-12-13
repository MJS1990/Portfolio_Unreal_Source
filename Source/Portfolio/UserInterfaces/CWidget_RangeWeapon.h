#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_RangeWeapon.generated.h"

UCLASS()
class PORTFOLIO_API UCWidget_RangeWeapon : public UUserWidget
{
	GENERATED_BODY()
		
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Magazine")
		uint8 MaxMagazineCount;

	UPROPERTY(BlueprintReadOnly, Category = "Magazine")
		uint8 CurrentMagazineCount;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateMagazineCount();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateSpreadRange(float InRadius, float InMaxRadius);
	void SetMaxMagazineCount(uint8 Count) { MaxMagazineCount = Count; }
	void SetCurrentMagazineCount(uint8 Count) { CurrentMagazineCount = Count; }

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
