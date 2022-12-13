#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponStructures.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBehavior);

UCLASS()
class PORTFOLIO_API UCEquipment : public UObject
{
	GENERATED_BODY()
	
public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
		void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
		void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
		void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
		void Unequip_Implementation();

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentBehavior OnBeginEquip;

	UPROPERTY(BlueprintAssignable)
		FEquipmentBehavior OnUnequip;

protected:
	UPROPERTY(BlueprintReadOnly)
		FEquipmentData Data;

private:
	class ACharacter* OwnerCharacter;

private:
	class UCStatusComponent* Status;
	class UCStateComponent* State;
	
public:
	FORCEINLINE bool GetBeginEquip() { return bBeginEquip; }

private:
	bool bBeginEquip;
	bool bEquipped;
};
