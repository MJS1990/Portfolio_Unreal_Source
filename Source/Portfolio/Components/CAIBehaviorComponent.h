#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIBehaviorType : uint8
{
	Wait, Approach, Action, Patrol, Hitted, Avoid, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIBehaviorTypeChanged, EAIBehaviorType, InPrevType, EAIBehaviorType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		FName BehaviorKey = "Behavior";

	UPROPERTY(EditDefaultsOnly)
		FName PlayerKey = "Player";

	UPROPERTY(EditDefaultsOnly)
		FName LocationKey = "EQS_Location";
	
	UPROPERTY(EditDefaultsOnly)
		FName LocationKey2 = "Player_Location";

public:
	UFUNCTION(BlueprintPure)
		bool IsWaitMode();

	UFUNCTION(BlueprintPure)
		bool IsApproachMode();

	UFUNCTION(BlueprintPure)
		bool IsActionMode();

	UFUNCTION(BlueprintPure)
		bool IsPatrolMode();

	UFUNCTION(BlueprintPure)
		bool IsHittedMode();

	UFUNCTION(BlueprintPure)
		bool IsAvoidMode();

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	UCAIBehaviorComponent();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();

	class ACPlayer* GetTargetPlayer();
	void SetTargetPlayer(AActor* actor);
	FVector GetLocation();

	void UpdatePlayerLocation();

protected:
	virtual void BeginPlay() override;

private:
	EAIBehaviorType GetType();
	void ChangeType(EAIBehaviorType InType);

public:
	FAIBehaviorTypeChanged OnBehaviorTypeChanged;

private:
	class UBlackboardComponent* Blackboard;
};
