#include "CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Weapons/CEquipment.h"

#include "Components/CAIBehaviorComponent.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";
	CurrentType = Type;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);
		
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (Type == EWeaponType::Fist)
	{
		if (!(weapon->IsFistMode()))
		{
			weapon->SetFistMode();
			CurrentType = EWeaponType::Fist;
		}
	}
	else if (Type == EWeaponType::Sword)
	{
		if (!(weapon->IsSwordMode()))
		{
			weapon->SetSwordMode();
			CurrentType = EWeaponType::Sword;
		}
	}
	else if (Type == EWeaponType::Rifle)
	{
		if (!(weapon->IsRifleMode()))
		{
			weapon->SetRifleMode();
			CurrentType = EWeaponType::Rifle;
		}
	}
	else if (Type == EWeaponType::Max)
	{
		if (!(weapon->IsUnarmedMode()))
		{
			weapon->SetUnarmedMode();
			CurrentType = EWeaponType::Max;
		}
	}

	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
		
	if (state->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(ai);

	if (weapon->GetCurrentWeapon()->GetEquipment()->GetBeginEquip() == false)
		weapon->GetCurrentWeapon()->GetEquipment()->Begin_Equip();
	
	weapon->GetCurrentWeapon()->GetEquipment()->End_Equip();

	return EBTNodeResult::Aborted;
}
