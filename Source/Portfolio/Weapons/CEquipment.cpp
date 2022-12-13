#include "Weapons/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (Data.bCanMove == false)
		Status->SetStop();

	if (!!Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRatio);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bUseControlRotation)
		Status->EnableControlRotation();
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnBeginEquip.IsBound())
		OnBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	State->SetIdleMode();
	Status->SetMove();
}

void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;
	Status->DisableControlRotation();

	if (OnUnequip.IsBound())
		OnUnequip.Broadcast();
}