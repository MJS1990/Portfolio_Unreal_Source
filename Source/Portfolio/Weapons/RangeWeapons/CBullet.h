#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/WeaponStructures.h"
#include "CBullet.generated.h"

UCLASS()
class PORTFOLIO_API ACBullet : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Impact")
		class UMaterialInstanceConstant* Decal;

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
		class UNiagaraSystem* Particle;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

protected:
	UPROPERTY(EditAnywhere)
		FHitData HitData;

public:
	ACBullet();

	void Shoot(const FVector& InDirection);
	void Impact(FVector Start, FVector End, class ACharacter* OwnerCharacter);

	void SetPlayer(class ACPlayer* p) { player = p; }

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	TArray<class ACharacter*> Hitted;

private:
	class ACPlayer* player;
};
