#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeapon.h"
#include "Components/TimelineComponent.h"
#include "CRifle.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		float TargetArmLength = 150;

	UPROPERTY(EditDefaultsOnly)
		FVector SocketOffset = FVector(0, 60, 0);
	
	UPROPERTY(EditDefaultsOnly)
		bool bEnableCameraLag = false;	
};

UCLASS()
class PORTFOLIO_API ACRifle : public ACWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float ConeAngle = 0.75f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float PitchAngle = 0.25f; 

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float LimitPitchAngle = 0.25f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float AimDistance = 3000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		float CurrentPitchAngle = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class UNiagaraSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
		TSubclassOf<class UMatineeCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
		class USoundWave* Sound;

	
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		class UParticleSystem* EjectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		TSubclassOf<class ACBullet> BulletClass;



	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		uint8 MaxMagazineCount = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		class UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		float ReloadMontage_PlayRate = 1.5;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName MagazineBoneName;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		TSubclassOf<class ACMagazine> MagazineClass;

	UPROPERTY(EditDefaultsOnly, Category = "Magazine")
		FName AttachedMagazineSocketName;

	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		TSubclassOf<class UCWidget_RangeWeapon> WidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		float SpreadSpeed = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		float MaxSpreadAligment = 3.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Equip")
		FName HolsterSocketName;

public:
	ACRifle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void Equip() override;
	void Unequip() override;

	/////////////////////////////////
public:
	//Fire
	void DoAction_RT() override;
	void Off_DoAction_RT() override;
	void End_DoAction_RT() override;

	//Aim
	void DoAction_LT() override;
	void Off_DoAction_LT() override;
	void End_DoAction_LT() override;

	//Reload
	void DoAction_X() override;

public:
	void SetAutoFire();
	void BeginShoot();

public:
	bool GetIsAiming() { return bAiming; }
	bool GetIsFire() { return bFire; }
	bool GetIsAuto() { return bAuto; }


//Reload, Magazine//////////////////////////////////////////
public:
	FORCEINLINE uint8 GetCurrMagazineCount() { return CurrMagazineCount; }
	FORCEINLINE uint8 GetMaxMagazineCount() { return MaxMagazineCount; }

	void Reload();

	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();

private:
	bool bReload = true;
	uint8 CurrMagazineCount;
	class ACMagazine* Magazine;
/////////////////////////////////////////////////////////////////

private:
	int32 Index;

	TArray<class ACharacter*> Hitted;

private:
	bool bAuto = true;
	bool bFire = false;
	bool bAim = false;

public:
	UFUNCTION()
		void Aiming(float FOV);

	void Begin_Aim();
	void End_Aim();

	bool IsAvaliableZoom();

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	FAimData OriginAimData;

	bool bAiming;
	FTimeline Timeline;

	class UCurveFloat* Curve;
	float CurveSpeed = 200;

private:
	class UCWidget_RangeWeapon* Widget;
	float CurrSpreadRadius;
	float LastAddSpreadTime;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Magazine")
		bool bVisibleMagazine = false;

};