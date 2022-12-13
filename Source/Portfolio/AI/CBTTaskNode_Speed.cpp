#include "AI/CBTTaskNode_Speed.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);


	float speed = 0;
	switch (Type)
	{
		case ESpeedType::Walk: speed = status->GetWalkSpeed(); break;
		case ESpeedType::Run: speed = status->GetRunSpeed(); break;
		case ESpeedType::Sprint: speed = status->GetSprintSpeed(); break;
	}

	ai->GetCharacterMovement()->MaxWalkSpeed = speed;

	return EBTNodeResult::Succeeded;
}
