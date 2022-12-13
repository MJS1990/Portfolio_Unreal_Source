#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class PORTFOLIO_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 2;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthBar;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UCAIPatrolComponent* Patrol;

public:
	FORCEINLINE uint8 GetTeamId() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

	class UCWeaponComponent* GetWeapon() { return Weapon; }

public:
	ACEnemy_AI();

protected:
	virtual void Hitted();
	void Dead();

public:
	virtual void End_Hitted() override;

private:
	class UCWidget_Base* Widget;

	float MaxHP;
	float CurrentHP;
};
