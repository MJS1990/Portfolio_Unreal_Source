#include "Notifies/CAnimNotifyState_SkillCollision.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/CAttachment.h"

FString UCAnimNotifyState_SkillCollision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_SkillCollision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());
	CheckNull(weapon->GetCurrentWeapon()->GetSkillAttachment());

	weapon->GetCurrentWeapon()->GetSkillAttachment()->OnCollision();

}

void UCAnimNotifyState_SkillCollision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());
	CheckNull(weapon->GetCurrentWeapon()->GetSkillAttachment());

	weapon->GetCurrentWeapon()->GetSkillAttachment()->OffCollision();
}