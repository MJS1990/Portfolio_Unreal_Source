#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponStructures.h"
#include "Weapons/CEquipment.h"
#include "CWeapon.generated.h"

UCLASS()
class PORTFOLIO_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = "IK")
		FVector RightHandLocation;

	UPROPERTY(EditDefaultsOnly, Category = "IK")
		FVector LeftHandLocation;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class ACAttachment* GetSkillAttachment() { return SkillAttachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE TArray<FDoActionData> GetDoActionDatas() { return DoActionDatas; }
	FORCEINLINE TArray<FHitData> GetHitDatas() { return HitDatas; }

	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }

protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> SkillAttachmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
		TArray<FHitData> HitDatas;

protected:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class ACAttachment* SkillAttachment;

	UPROPERTY()
		class UCEquipment* Equipment;

public:	
	ACWeapon();

public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnBeginEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnEquip();

protected:
	UFUNCTION(BlueprintCallable)
		void AttachToSkeletalMesh(FName InSocketName);

	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;
	
public:
	virtual void Equip() 
	{
		Equipment->Equip(); 
		bEquip = true;
	}
	virtual void Unequip() 
	{
		Equipment->Unequip(); 
		bEquip = false;
	}
	
	const bool IsEquip() { return bEquip; }
	
protected:
	bool bEquip = false;

public:
	virtual void DoAction_A();
	virtual void Begin_DoAction_A();
	virtual void End_DoAction_A();

	virtual void DoAction_B();
	virtual void Begin_DoAction_B();
	virtual void End_DoAction_B();

	virtual void DoAction_X();
	virtual void Begin_DoAction_X();
	virtual void End_DoAction_X();

	virtual void DoAction_Y();
	virtual void Begin_DoAction_Y();
	virtual void End_DoAction_Y();

	virtual void DoAction_LT();
	virtual void Begin_DoAction_LT();
	virtual void End_DoAction_LT();
	//TODO : 에임 테스트 후 수정
	virtual void Off_DoAction_LT() {}
	virtual void Off_DoAction_RT() {}

	virtual void DoAction_RT();
	virtual void Begin_DoAction_RT();
	virtual void End_DoAction_RT();

public:
	UFUNCTION()
		virtual void OnAttachmentCollision() { }

	UFUNCTION()
		virtual void OffAttachmentCollision() { }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) { }

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) { }

protected:
	class UWorld* World;

	class UCStateComponent* State;
	class UCStatusComponent* Status;


protected:
	bool bBeginAction;
};
