#include "CBTTaskNode_Hitted.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_Hitted::UCBTTaskNode_Hitted()
{
	bNotifyTick = true;

	NodeName = "Hitted";

	CHelpers::GetClass<ACPlayer>(&PlayerCharacter, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
}

EBTNodeResult::Type UCBTTaskNode_Hitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	controller->StopMovement();
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	EAIBehaviorType type = (EAIBehaviorType)controller->GetBlackboardComponent()->GetValueAsEnum("Behavior");

	if (type != EAIBehaviorType::Hitted)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
