#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist = 0,
	Sword,
	Rifle,
	Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Weapons")
		TMap<EWeaponType, TSubclassOf<class ACWeapon>> WeaponMap;
	
public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsFistMode() { return Type == EWeaponType::Fist; }
	FORCEINLINE bool IsSwordMode() { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsRifleMode() { return Type == EWeaponType::Rifle; }


public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetUnarmedMode();
	void SetFistMode();
	void SetSwordMode();
	void SetRifleMode();
	
	void DoAction_A();
	void DoAction_B();
	void DoAction_X();
	void DoAction_Y();
	void DoAction_LT();
	void OffDoAction_LT();
	void DoAction_RT();
	void OffDoAction_RT();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);
	
	void SetUseControlRotation(bool bInUse = false);

public:
	void SetOwner(class ACharacter* owner) { OwnerCharacter = owner; }

	class ACWeapon* GetCurrentWeapon();

	bool IsAiming();
	bool IsFire();
	FVector GetLeftHandLocation();

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	class ACharacter* OwnerCharacter;

	EWeaponType Type = EWeaponType::Max;
	
public:
	TMap<EWeaponType, class ACWeapon*> CurrentWeaponMap;
};
