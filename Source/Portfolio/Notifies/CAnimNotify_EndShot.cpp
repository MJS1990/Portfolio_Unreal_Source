#include "Notifies/CAnimNotify_EndShot.h"
#include "Global.h"
#include "Animation/AnimMontage.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/RangeWeapons/CRifle.h"

FString UCAnimNotify_EndShot::GetNotifyName_Implementation() const
{
	return "End_Shot";
}

void UCAnimNotify_EndShot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(state);


	if (state->IsQuitActionMode())
		weapon->GetCurrentWeapon()->End_DoAction_RT();

	bool bAuto = Cast<ACRifle>(weapon->GetCurrentWeapon())->GetIsAuto();
	//if (!state->IsQuitActionMode())
	//{
	//
	//}
	//else if (state->IsQuitActionMode() && bAuto)
	//{
	//	weapon->GetCurrentWeapon()->GetDoAction()->End_DoAction_RT();
	//}

	weapon->GetCurrentWeapon()->End_DoAction_RT();

}
