#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCAIPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "PatrolPath")
		class ACPatrolPath* Patrol;

	UPROPERTY(EditAnywhere, Category = "Path")
		int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;

	UPROPERTY(EditAnywhere, Category = "Path")
		float AcceptanceDistance = 10;

public:
	FORCEINLINE bool IsValidPatrol() { return Patrol != nullptr; }

public:
	UCAIPatrolComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceDistance);
	void Update();
	
};
