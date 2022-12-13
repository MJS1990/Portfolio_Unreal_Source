#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Approach.generated.h"

UCLASS()
class PORTFOLIO_API UCBTTaskNode_Approach : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Approach();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "TargetPlayer")
		TSubclassOf<class ACPlayer> Player;
};
