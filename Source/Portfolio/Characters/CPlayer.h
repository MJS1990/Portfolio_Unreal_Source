#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//Hit////////////////////////////////////////////////////
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
////////////////////////////////////////////////////////
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTFOLIO_API ACPlayer 
	: public ACharacter
	, public IGenericTeamAgentInterface, public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 1;

	UPROPERTY(EditDefaultsOnly, Category = "CameraSpeed")
		float HorizontalLook = 45;

	UPROPERTY(EditDefaultsOnly, Category = "CameraSpeed")
		float VerticalLook = 45;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* TakeDamageMontages;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
	
	UPROPERTY(VisibleDefaultsOnly)
		class UCZoomComponent* Zoom;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TSubclassOf<class UCWidget_Base> HUDClass;

public:
	FORCEINLINE virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

public:
	ACPlayer();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnHorizontalLook(float InAxis);

	void OnRun();
	void OffRun();

	void OnAction_A();
	void OnAction_B();
	void OnAction_X();
	void OnAction_Y();
	void OnAction_LT();
	void OffAction_LT();
	void OnAction_RT();
	void OffAction_RT();

	void OffAction();

public:
	float GetCurrentAxisValue() { return AxisValue; }

private:
	void SetFistWeaponType();
	void SetRifleWeaponType();
	
private:
	float AxisValue;

private:
	class UCWidget_Base* HUD;

//Hit////////////////////////////////////////////////////////////////
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void Hitted();
	void Dead();

public:
	virtual void End_Hitted() override;
	virtual void End_Dead() override;

private:
	struct FDamageData
	{
		float Amount;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} Damage;

private:
	float MaxHP;
	bool bDead = false;
};
