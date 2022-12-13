#include "Characters/CEnemy_AI.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CAIPatrolComponent.h"
#include "Components/CWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "UserInterfaces/CWidget_Base.h"
#include "AIController.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent(this, &Patrol, "Patrol");

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	Weapon->SetOwner(this);
	
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthBar, "HealthBar", RootComponent);
	MaxHP = Status->GetHealth();
	CurrentHP = Status->GetHealth();

}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

	Widget = Cast<UCWidget_Base>(HealthBar->GetUserWidgetObject());
	Widget->UpdateHP(1, 1);
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemy_AI::Hitted()
{
	Super::Hitted();

	CurrentHP = Status->GetHealth();
	
	if (!!HealthBar)
		Widget->UpdateHP(CurrentHP, 100);

	AAIController* controller = Cast<AAIController>(GetController());
	controller->GetBlackboardComponent()->SetValueAsEnum("Behavior", (int32)EAIBehaviorType::Hitted);
}

void ACEnemy_AI::End_Hitted()
{
	Super::End_Hitted();

	AAIController* controller = Cast<AAIController>(GetController());
	controller->GetBlackboardComponent()->SetValueAsEnum("Behavior", (int32)EAIBehaviorType::Wait);
}

void ACEnemy_AI::Dead()
{
	Super::Dead();

	AAIController* controller = Cast<AAIController>(GetController());
}