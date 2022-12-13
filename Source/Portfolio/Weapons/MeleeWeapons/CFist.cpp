#include "Weapons/MeleeWeapons/CFist.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Characters/CEnemy_AI.h"

ACFist::ACFist()
{
	CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/Weapons/Melee/Fist/BP_CameraShake_Fist.BP_CameraShake_Fist_C'");	
}

void ACFist::BeginPlay()
{
	Super::BeginPlay();

	bSkill = false;
}

void ACFist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/////////////////////////////////////////////////////////////////////
void ACFist::DoAction_RT()
{
	Super::DoAction_RT();
	CheckFalse(DoActionDatas.Num() > 0);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}
	CheckFalse(State->IsIdleMode());
	bSkill = false;
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void ACFist::Begin_DoAction_RT()
{
	Super::Begin_DoAction_RT();
	CheckFalse(bExist);
	if (!!CameraShakeClass && !(Cast<ACEnemy_AI>(OwnerCharacter)))
		OwnerCharacter->GetController<APlayerController>()->PlayerCameraManager->StartCameraShake(CameraShakeClass);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void ACFist::End_DoAction_RT()
{
	Super::End_DoAction_RT();

	Index = 0;
}

///////////////////////////////////////////////////////////////////
void ACFist::DoAction_LT()
{
	Super::DoAction_LT();
	bSkill = true;

	DoSkillData.DoAction(OwnerCharacter);
}

void ACFist::Begin_DoAction_LT()
{
	Super::Begin_DoAction_LT();

}

void ACFist::End_DoAction_LT()
{
	Super::End_DoAction_LT();

}

/////////////////////////////////////////////////////////////////////
void ACFist::OffAttachmentCollision()
{
	Super::OffAttachmentCollision();

	float angle = -2.0f;
	ACharacter* candidate = nullptr;
	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}
	}

	Hitted.Empty();
}

void ACFist::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);

	if (bSkill)
		SkillHitData.SendDamage(InAttacker, InAttackCauser, InOther);
	else
		HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
}