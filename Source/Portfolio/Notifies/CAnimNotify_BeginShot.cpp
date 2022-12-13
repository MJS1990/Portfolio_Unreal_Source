#include "Notifies/CAnimNotify_BeginShot.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/RangeWeapons/CRifle.h"

FString UCAnimNotify_BeginShot::GetNotifyName_Implementation() const
{
	return "Begin_Shot";
}

void UCAnimNotify_BeginShot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());

	weapon->GetCurrentWeapon()->Begin_DoAction_RT();
}
