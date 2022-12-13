#include "CAIController.h"
#include "Global.h"
#include "CPlayer.h"
#include "CEnemy_AI.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CPlayer.h"

ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 120;
	Sight->SetMaxAge(2);
	
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	
	SetGenericTeamId(Enemy->GetTeamId());
	
	CheckNull(Blackboard);
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	ACPlayer* player = nullptr;

	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);

		if (!!player)
			break;
	}

	Blackboard->SetValueAsObject("Player", player);
}

void ACAIController::SetHittedSight()
{
	Sight->SightRadius = 6000;
	Sight->LoseSightRadius = 8000;
	Sight->PeripheralVisionAngleDegrees = 180;
	Sight->SetMaxAge(8);

	Perception->ConfigureSense(*Sight);
	Perception->Activate(true);
}

void ACAIController::SetNormalSight()
{
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 120;
	Sight->SetMaxAge(2);
	
	Perception->ConfigureSense(*Sight);
	Perception->Activate(true);
}