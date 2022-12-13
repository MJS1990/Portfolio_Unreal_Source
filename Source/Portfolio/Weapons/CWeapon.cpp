#include "Weapons/CWeapon.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"

ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	
	AttachmentClass = ACAttachment::StaticClass();
	SkillAttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void ACWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	World = OwnerCharacter->GetWorld();

	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = OwnerCharacter;

		Attachment = OwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);

		if(!!SkillAttachmentClass)
			SkillAttachment = OwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(SkillAttachmentClass, params);
	}

	if (!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(OwnerCharacter, EquipmentData);

		if (!!Attachment)
		{
			Equipment->OnBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnUnequip.AddDynamic(Attachment, &ACAttachment::OnUnEquip);
		}
	}

	if (!!Attachment)
	{
		Attachment->OnAttachmentCollision.AddDynamic(this, &ACWeapon::OnAttachmentCollision);
		Attachment->OffAttachmentCollision.AddDynamic(this, &ACWeapon::OffAttachmentCollision);

		Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &ACWeapon::OnAttachmentBeginOverlap);
		Attachment->OnAttachmentEndOverlap.AddDynamic(this, &ACWeapon::OnAttachmentEndOverlap);
	}

	if (!!SkillAttachment)
	{
		SkillAttachment->OnAttachmentCollision.AddDynamic(this, &ACWeapon::OnAttachmentCollision);
		SkillAttachment->OffAttachmentCollision.AddDynamic(this, &ACWeapon::OffAttachmentCollision);

		SkillAttachment->OnAttachmentBeginOverlap.AddDynamic(this, &ACWeapon::OnAttachmentBeginOverlap);
		SkillAttachment->OnAttachmentEndOverlap.AddDynamic(this, &ACWeapon::OnAttachmentEndOverlap);
	}


	Equipment->OnBeginEquip.AddDynamic(this, &ACWeapon::OnBeginEquip);
	Equipment->OnUnequip.AddDynamic(this, &ACWeapon::OnUnEquip);
}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACWeapon::AttachToSkeletalMesh(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACWeapon::DoAction_A()
{

}

void ACWeapon::Begin_DoAction_A()
{
	bBeginAction = true;
}

void ACWeapon::End_DoAction_A()
{
	bBeginAction = false;

	State->SetIdleMode();
	Status->SetMove();
	Status->DisableFixedCamera();
}


void ACWeapon::DoAction_B()
{

}

void ACWeapon::Begin_DoAction_B()
{
	bBeginAction = true;
}

void ACWeapon::End_DoAction_B()
{
	bBeginAction = false;

	State->SetIdleMode();
	Status->SetMove();
	Status->DisableFixedCamera();
}


void ACWeapon::DoAction_X()
{

}

void ACWeapon::Begin_DoAction_X()
{
	bBeginAction = true;
}

void ACWeapon::End_DoAction_X()
{
	bBeginAction = false;

	State->SetIdleMode();
	Status->SetMove();
	Status->DisableFixedCamera();
}

void ACWeapon::DoAction_Y()
{

}

void ACWeapon::Begin_DoAction_Y()
{
	bBeginAction = true;
}

void ACWeapon::End_DoAction_Y()
{
	bBeginAction = false;

	State->SetIdleMode();
	Status->SetMove();
	Status->DisableFixedCamera();
}

void ACWeapon::DoAction_LT()
{

}

void ACWeapon::Begin_DoAction_LT()
{
	bBeginAction = true;
}

void ACWeapon::End_DoAction_LT()
{
	bBeginAction = false;

	State->SetIdleMode();
	Status->SetMove();
	Status->DisableFixedCamera();
}

void ACWeapon::DoAction_RT()
{

}

void ACWeapon::Begin_DoAction_RT()
{
	bBeginAction = true;
}

void ACWeapon::End_DoAction_RT()
{
	bBeginAction = false;

	State->SetIdleMode();
	Status->SetMove();
	Status->DisableFixedCamera();
}