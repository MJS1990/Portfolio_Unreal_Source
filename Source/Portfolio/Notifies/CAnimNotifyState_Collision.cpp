#include "Notifies/CAnimNotifyState_Collision.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/CAttachment.h"

FString UCAnimNotifyState_Collision::GetNotifyName_Implementation() const
{
	return "Collision";
}

void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());
	CheckNull(weapon->GetCurrentWeapon()->GetAttachment());

	weapon->GetCurrentWeapon()->GetAttachment()->OnCollision();

}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentWeapon());
	CheckNull(weapon->GetCurrentWeapon()->GetAttachment());

	weapon->GetCurrentWeapon()->GetAttachment()->OffCollision();
}