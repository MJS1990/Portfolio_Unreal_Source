#include "AI/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CAIPatrolComponent.h"
#include "NavigationSystem.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	bNotifyTick = true;

	NodeName = "Patrol";
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIPatrolComponent* patrol = CHelpers::GetComponent<UCAIPatrolComponent>(ai);

	if (!!patrol && patrol->IsValidPatrol())
	{
		float distance;
		FVector location;
		
		if (patrol->GetMoveTo(location, distance) == false)
			return EBTNodeResult::Failed;

		return EBTNodeResult::InProgress;
	}

	FNavLocation navLocation;

	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	bool bCheck = navSystem->GetRandomReachablePointInRadius(ai->GetActorLocation(), SearchRadius, navLocation);

	Location = navLocation.Location;
	return bCheck ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIPatrolComponent* patrol = CHelpers::GetComponent<UCAIPatrolComponent>(ai);

	EPathFollowingRequestResult::Type type = EPathFollowingRequestResult::Failed;

	if (!!patrol && patrol->IsValidPatrol())
	{
		float distance;
		FVector location;

		patrol->GetMoveTo(location, distance);
		type = controller->MoveToLocation(location, distance, false);
	}
	else
		type = controller->MoveToLocation(Location, 5, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		if (!!patrol && patrol->IsValidPatrol())
			patrol->Update();
	
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}
