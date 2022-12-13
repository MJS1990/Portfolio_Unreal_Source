#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CWeapon.h"
#include "CWeaponAnimInstance.generated.h"

UCLASS()
class PORTFOLIO_API UCWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WeaponData")
		bool bAiming;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "WeaponData")
		bool bFire;

public:
	UCWeaponAnimInstance();

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	TSubclassOf<ACWeapon> OwnerWeapon;
};