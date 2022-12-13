#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0,
	Roll,
	BackStep,
	Equip,
	Hitted,
	Dead,
	Action,
	QuitAction,
	SubAction,
	QuitSubAction,
	Max
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNextType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
	FORCEINLINE bool IsBackStepMode() { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsQuitActionMode() { return Type == EStateType::QuitAction; }
	FORCEINLINE bool IsSubActionMode() { return Type == EStateType::SubAction; }
	FORCEINLINE bool IsQuitSubActionMode() { return Type == EStateType::QuitSubAction; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	void SetIdleMode();
	void SetBackStepMode();
	void SetRollMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	
	void SetActionMode();
	void SetQuitActionMode();
	void SetSubActionMode();
	void SetQuitSubActionMode();

	void ChangeType(EStateType InType);

	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type = EStateType::Idle;
};
