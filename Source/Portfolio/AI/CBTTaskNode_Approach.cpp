#include "AI/CBTTaskNode_Approach.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"

UCBTTaskNode_Approach::UCBTTaskNode_Approach()
{
	bNotifyTick = true;

	NodeName = "Approach";
	CHelpers::GetClass<ACPlayer>(&Player, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
}


EBTNodeResult::Type UCBTTaskNode_Approach::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	CLog::Log("Execute Approach");

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Approach::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	CLog::Log("Tick Approach");

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	EAIBehaviorType type = (EAIBehaviorType)controller->GetBlackboardComponent()->GetValueAsEnum("Behavior");
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	
	ACPlayer* player = Cast<ACPlayer>(Player);

	if (player != nullptr)
	{
		FTransform t = player->GetTransform();
		FVector v = t.GetTranslation();
	}

	EPathFollowingRequestResult::Type PFType = EPathFollowingRequestResult::Failed;                          
	
	if (PFType == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

		return;
	}

	if (PFType == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UCBTTaskNode_Approach::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	CLog::Log("Abort Approach");

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	
	return EBTNodeResult::Aborted;
}