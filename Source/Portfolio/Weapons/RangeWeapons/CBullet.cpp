#include "Weapons/RangeWeapons/CBullet.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/DecalComponent.h"
#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


ACBullet::ACBullet()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Capsule);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Capsule->SetRelativeRotation(FRotator(90, 0, 0));
	Capsule->SetCapsuleHalfHeight(50);
	Capsule->SetCapsuleRadius(2);
	Capsule->SetCollisionProfileName("BlockAllDynamic");

	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "StaticMesh'/Game/Meshes/Sphere.Sphere'");
	Mesh->SetStaticMesh(mesh);
	Mesh->SetRelativeScale3D(FVector(1, 0.025f, 0.05f));
	Mesh->SetRelativeRotation(FRotator(90, 0, 0));

	Projectile->InitialSpeed = 2e+4f;
	Projectile->MaxSpeed = 2e+4f;
	Projectile->ProjectileGravityScale = 0;

	CHelpers::GetAsset<UMaterialInstanceConstant>(&Decal, "MaterialInstanceConstant'/Game/WeaponKit/FX/MI_BulletHole_Decal_WK.MI_BulletHole_Decal_WK'");
	CHelpers::GetAsset<UNiagaraSystem>(&Particle, "NiagaraSystem'/Game/WeaponKit/FX/NS_BulletImpactPuff_WK.NS_BulletImpactPuff_WK'");
}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();
	
	Capsule->OnComponentHit.AddDynamic(this, &ACBullet::OnHit);
}

void ACBullet::Shoot(const FVector& InDirection)
{
	Projectile->Velocity = InDirection * Projectile->InitialSpeed;
}

void ACBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<ACEnemy_AI>(OtherActor) && !(Cast<ACEnemy_AI>(OtherActor)->IsDead()))
	{
		if(!!HitData.Montage)
			Cast<ACEnemy_AI>(OtherActor)->PlayAnimMontage(HitData.Montage);
	
		if(!!Capsule)
			HitData.SendDamage(player, Cast<AActor>(this), Cast<ACharacter>(OtherActor));
	}
	
	Destroy();
}

void ACBullet::Impact(FVector Start, FVector End, ACharacter* OwnerCharacter)
{
	TArray<AActor *> ignores;
	ignores.Add(Cast<AActor>(OwnerCharacter));

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);
	if (hitResult.bBlockingHit)
	{
		FRotator rotation = hitResult.ImpactNormal.Rotation();

		if (!!Decal)
		{
			UDecalComponent* decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), Decal, FVector(5), hitResult.Location, rotation, 10);
			decal->SetFadeScreenSize(0);
		}
	
		if (!!Particle)
		{
			FRotator impactRotation = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particle, hitResult.Location, impactRotation, hitResult.GetActor()->GetActorScale3D());
		}
		//Destroy(); //TODO : 체크할것
	}
}

