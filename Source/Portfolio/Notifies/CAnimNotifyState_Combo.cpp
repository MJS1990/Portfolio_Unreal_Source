#include "Notifies/CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/MeleeWeapons/CFist.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());

	ACFist* combo = Cast<ACFist>(weapon->GetCurrentWeapon());
	CheckNull(combo);

	combo->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());

	ACFist* combo = Cast<ACFist>(weapon->GetCurrentWeapon());
	CheckNull(combo);

	combo->DisableCombo();
}
