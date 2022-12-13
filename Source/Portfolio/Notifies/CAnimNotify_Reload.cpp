#include "CAnimNotify_Reload.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/RangeWeapons/CRifle.h"

FString UCAnimNotify_Reload::GetNotifyName_Implementation() const
{
	return "Reload";
}

void UCAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weaponComponent = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weaponComponent);
	
	ACRifle* weapon = Cast<ACRifle>(weaponComponent->GetCurrentWeapon());

	switch (ActionType)
	{
		case EReloadActionType::Eject: weapon->Eject_Magazine(); break;
		case EReloadActionType::Spawn: weapon->Spawn_Magazine(); break;
		case EReloadActionType::Load: weapon->Load_Magazine(); break;
		case EReloadActionType::End: weapon->End_Reload(); break;
	}

}
