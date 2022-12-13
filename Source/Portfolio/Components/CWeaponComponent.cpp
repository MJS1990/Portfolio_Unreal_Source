#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CPlayer.h"
#include "Weapons/CWeapon.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/RangeWeapons/CRifle.h"


UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(OwnerCharacter);

	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;

	for (TPair<EWeaponType, TSubclassOf<ACWeapon>> pair : WeaponMap)
	{
		if (!!pair.Value)
		{
			ACWeapon* weapon = OwnerCharacter->GetWorld()->SpawnActor<ACWeapon>(pair.Value, params);
			CurrentWeaponMap.Add(pair.Key, weapon);
		}
	}
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCWeaponComponent::SetUnarmedMode()
{
	GetCurrentWeapon()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetRifleMode()
{
	SetMode(EWeaponType::Rifle);
}

void UCWeaponComponent::DoAction_A()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->DoAction_A();	
	}
}

void UCWeaponComponent::DoAction_B()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->DoAction_B();
	}
}

void UCWeaponComponent::DoAction_X()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->DoAction_X();
	}
}

void UCWeaponComponent::DoAction_Y()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->DoAction_Y();
	}
}

void UCWeaponComponent::DoAction_LT()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->DoAction_LT();
	}
}

void UCWeaponComponent::OffDoAction_LT()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->Off_DoAction_LT();
	}
}

void UCWeaponComponent::DoAction_RT()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->DoAction_RT();
	}
}

void UCWeaponComponent::OffDoAction_RT()
{
	if (Type != EWeaponType::Max)
	{
		CheckNull(GetCurrentWeapon())
			GetCurrentWeapon()->Off_DoAction_RT();
	}
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetCurrentWeapon()->Unequip();
	}

	CheckNull(WeaponMap[InType]);
	CurrentWeaponMap[InType]->Equip();
	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;
	
	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(prevType, InType);
}

ACWeapon* UCWeaponComponent::GetCurrentWeapon()
{
	CheckFalseResult(CurrentWeaponMap.Contains(Type), nullptr);

	return *CurrentWeaponMap.Find(Type);
}

void UCWeaponComponent::SetUseControlRotation(bool bInUse)
{
	if (bInUse)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

		return;
	}

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

bool UCWeaponComponent::IsAiming()
{
	return Cast<ACRifle>(GetCurrentWeapon())->GetIsAiming();
}

bool UCWeaponComponent::IsFire()
{
	return Cast<ACRifle>(GetCurrentWeapon())->GetIsFire();
}

FVector UCWeaponComponent::GetLeftHandLocation()
{
	CheckNullResult(GetCurrentWeapon(), FVector::ZeroVector);

	return GetCurrentWeapon()->GetLeftHandLocation();
}