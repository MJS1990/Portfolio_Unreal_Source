#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Stop = 0,
	Walk,
	Run,
	Sprint,
	Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 0, 200, 400, 600 };

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;

public:
	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

	FORCEINLINE float GetHealth() { return Health; }

	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE void SetMove() { SetSpeed(ESpeedType::Run); }
	FORCEINLINE void SetStop() { SetSpeed(ESpeedType::Stop); }

	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetSpeed(ESpeedType InType);
	void Damage(float InAmouont);

	void EnableControlRotation();
	void DisableControlRotation();

private:
	class ACharacter* OwnerCharacter;

	float Health;
	bool bCanMove = true;
	bool bFixedCamera;
};