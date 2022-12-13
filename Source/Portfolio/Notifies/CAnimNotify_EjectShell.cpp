#include "Notifies/CAnimNotify_EjectShell.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/RangeWeapons/CRifle.h"

FString UCAnimNotify_EjectShell::GetNotifyName_Implementation() const
{
	return "Eject_Shell";
}

void UCAnimNotify_EjectShell::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon->GetCurrentWeapon());

	Cast<ACRifle>(weapon->GetCurrentWeapon())->BeginShoot();
}