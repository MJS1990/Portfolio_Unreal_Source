#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "Weapons/WeaponStructures.h"
#include "CFist.generated.h"

UCLASS()
class PORTFOLIO_API ACFist : public ACWeapon
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

protected:
	UPROPERTY(EditAnywhere)
		FDoActionData DoSkillData;
	
	UPROPERTY(EditAnywhere)
		FHitData SkillHitData;

public:
	ACFist();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction_RT() override;
	virtual void Begin_DoAction_RT() override;
	virtual void End_DoAction_RT() override;

	virtual void DoAction_LT() override;
	virtual void Begin_DoAction_LT() override;
	virtual void End_DoAction_LT() override;

public:
	virtual void OffAttachmentCollision() override;
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) override;

	FORCEINLINE bool GetExist() { return bExist; }

private:
	int32 Index;
	
	bool bEnable;
	bool bExist;

	bool bSkill;

	TArray<class ACharacter*> Hitted;
};
