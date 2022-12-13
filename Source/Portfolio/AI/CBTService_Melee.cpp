#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

#include "Components/CWeaponComponent.h"
#include "Weapons/CWeapon.h"
#include "Characters/CPlayer.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

	CHelpers::GetClass<ACPlayer>(&PlayerCharacter, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	if (!!state && state->IsDeadMode()) return;

	ACPlayer* player = behavior->GetTargetPlayer();
	float distance = ai->GetDistanceTo(player);

	if (state->IsHittedMode() == true && behavior->IsHittedMode() == false)
		behavior->SetHittedMode();
	
	if (state->IsHittedMode() == true)
		controller->SetHittedSight();

	if (player != nullptr || behavior->IsHittedMode() == true)
	{
		behavior->SetActionMode();
	
		return;
	}

	if (behavior->IsActionMode()) behavior->SetWaitMode();

	if (player == nullptr)
	{
		behavior->SetPatrolMode();
		if (!!(ai->GetWeapon()) && state->IsEquipMode())
			ai->GetWeapon()->GetCurrentWeapon()->GetEquipment()->Unequip();

		controller->SetNormalSight();
	}
}
