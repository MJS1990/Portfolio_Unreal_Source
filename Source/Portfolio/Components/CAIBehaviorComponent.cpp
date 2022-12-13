#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAIBehaviorComponent::UCAIBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
		
}

void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UCAIBehaviorComponent::IsWaitMode()
{
	return GetType() == EAIBehaviorType::Wait;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetType() == EAIBehaviorType::Approach;
}

bool UCAIBehaviorComponent::IsActionMode()
{
	return GetType() == EAIBehaviorType::Action;
}

bool UCAIBehaviorComponent::IsPatrolMode()
{
	return GetType() == EAIBehaviorType::Patrol;
}

bool UCAIBehaviorComponent::IsHittedMode()
{
	return GetType() == EAIBehaviorType::Hitted;
}

bool UCAIBehaviorComponent::IsAvoidMode()
{
	return GetType() == EAIBehaviorType::Avoid;
}

void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeType(EAIBehaviorType::Wait);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIBehaviorType::Approach);
}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeType(EAIBehaviorType::Action);
}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeType(EAIBehaviorType::Patrol);
}

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeType(EAIBehaviorType::Hitted);
}

void UCAIBehaviorComponent::SetAvoidMode()
{
	ChangeType(EAIBehaviorType::Avoid);
}

void UCAIBehaviorComponent::ChangeType(EAIBehaviorType InType)
{
	EAIBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InType);
}

ACPlayer* UCAIBehaviorComponent::GetTargetPlayer()
{
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

void UCAIBehaviorComponent::SetTargetPlayer(AActor* actor)
{
	Blackboard->SetValueAsObject(PlayerKey, Cast<UObject>(actor));
}

FVector UCAIBehaviorComponent::GetLocation()
{
	return Blackboard->GetValueAsVector(LocationKey);
}

EAIBehaviorType UCAIBehaviorComponent::GetType()
{
	return (EAIBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}

void UCAIBehaviorComponent::UpdatePlayerLocation()
{
	if(GetTargetPlayer() != nullptr)
		Blackboard->SetValueAsVector(LocationKey2, GetTargetPlayer()->GetTransform().GetLocation());
	else
		Blackboard->SetValueAsVector(LocationKey2, FVector(0.0f, 0.0f, 0.0f));
}
