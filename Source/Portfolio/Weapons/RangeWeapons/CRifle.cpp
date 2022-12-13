#include"CRifle.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/CStateComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "Sound/SoundWave.h"
#include "Characters/CAnimInstance.h"
#include "UserInterfaces/CWidget_RangeWeapon.h"
#include "Characters/CPlayer.h"
#include "CBullet.h"
#include "CMagazine.h"

ACRifle::ACRifle()
{	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/WeaponKit/Weapons/RifleM4C/SK_RifleM4C_Body.SK_RifleM4C_Body'");
	SkeletalMesh->SetSkeletalMesh(mesh);

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Weapons/Range/Curve_Aim.Curve_Aim'");

	CHelpers::GetClass<ACBullet>(&BulletClass, "Blueprint'/Game/Weapons/Range/Rifle/BP_CBullet.BP_CBullet_C'");
	CHelpers::GetClass<ACMagazine>(&MagazineClass, "Blueprint'/Game/Weapons/Range/Rifle/BP_CMagazine_Rifle.BP_CMagazine_Rifle_C'");

	CHelpers::GetClass<UMatineeCameraShake>(&CameraShakeClass, "Blueprint'/Game/Weapons/Range/Rifle/BP_CameraShake.BP_CameraShake_C'");
	
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/TPSFPSCharacterSystem/Effects/ParticleSystems/P_BulletShellSFX.P_BulletShellSFX'");
	CHelpers::GetAsset<UNiagaraSystem>(&FlashParticle, "NiagaraSystem'/Game/WeaponKit/FX/NS_MuzzleFlash_WK.NS_MuzzleFlash_WK'");
	
	CHelpers::GetClass<UCWidget_RangeWeapon>(&WidgetClass, "WidgetBlueprint'/Game/UI/HUD_Crosshair/WBP_HUD_RangeWeapon.WBP_HUD_RangeWeapon_C'");
}

void ACRifle::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);

	FOnTimelineFloat CB_Timeline;
	CB_Timeline.BindUFunction(this, "Aiming");

	Timeline.AddInterpFloat(Curve, CB_Timeline);
	Timeline.SetPlayRate(CurveSpeed);

	if (!!WidgetClass)
	{
		Widget = CreateWidget<UCWidget_RangeWeapon, APlayerController>(OwnerCharacter->GetController<APlayerController>(), WidgetClass);
		Widget->AddToViewport();
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
	Widget->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAligment);
	Widget->SetMaxMagazineCount(MaxMagazineCount);
	Widget->SetCurrentMagazineCount(MaxMagazineCount);
	Widget->UpdateMagazineCount();

	CurrMagazineCount = MaxMagazineCount;
	FTransform transform;
	SkeletalMesh->GetSocketTransform(MagazineBoneName).GetRelativeTransform(transform);

	ACMagazine* magazine = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazine->FinishSpawning(transform);
}

void ACRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timeline.TickTimeline(DeltaTime);

	if (LastAddSpreadTime >= 0.0f)
	{
		if (GetWorld()->GetTimeSeconds() - LastAddSpreadTime >= 0.1f + 0.25f)
		{
			CurrSpreadRadius = 0;
			LastAddSpreadTime = 0;

			Widget->UpdateSpreadRange(CurrSpreadRadius, MaxSpreadAligment);
		}
	}
}

////////////////////////////////////////////

void ACRifle::DoAction_RT()
{
	if (!bReload) return;

	Super::DoAction_RT();
	CheckFalse(DoActionDatas.Num() > 0);

	bFire = true;

	CheckFalse(State->IsIdleMode());
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void ACRifle::Off_DoAction_RT()
{
	Super::Off_DoAction_RT();

	bFire = false;
	State->SetIdleMode();
}

void ACRifle::End_DoAction_RT()
{
	Super::End_DoAction_RT();

	Index = 0;
}

void ACRifle::DoAction_LT()
{
	if (!bReload) return;

	Super::DoAction_LT();
	Begin_Aim();
}

void ACRifle::Off_DoAction_LT()
{
	Super::Off_DoAction_LT();
	End_Aim();
}

void ACRifle::End_DoAction_LT()
{
	Super::End_DoAction_LT();
}

void ACRifle::DoAction_X()
{
	Super::DoAction_X();

	if(!!bReload)	
		Reload();
}


void ACRifle::SetAutoFire()
{
	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void ACRifle::BeginShoot()
{
	if (!bReload) return;

	if (!!EjectParticle)
		UGameplayStatics::SpawnEmitterAttached(EjectParticle, SkeletalMesh, "ShellEject");
	
	FVector location = SkeletalMesh->GetRelativeLocation();
	FRotator rotation = SkeletalMesh->GetRelativeRotation();
	FVector scale = SkeletalMesh->GetRelativeScale3D();
	
	if (!!FlashParticle)
		UNiagaraFunctionLibrary::SpawnSystemAttached(FlashParticle, SkeletalMesh, "MuzzleFlash", location, rotation, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
	
	if (!!CameraShakeClass)
		OwnerCharacter->GetController<APlayerController>()->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	
	FVector muzzleLocation = SkeletalMesh->GetSocketLocation("MuzzleFlash");

	if (!!Sound)
		UGameplayStatics::PlaySoundAtLocation(SkeletalMesh->GetWorld(), Sound, muzzleLocation);
	
	//Bullet
	FVector direction = Camera->GetForwardVector();
	FTransform transform = Camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;

	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, ConeAngle);
	FVector end = transform.GetLocation() + direction * AimDistance;

	if (CurrentPitchAngle <= LimitPitchAngle)
	{
		CurrentPitchAngle += PitchAngle * GetWorld()->GetDeltaSeconds();
		OwnerCharacter->AddControllerPitchInput(-CurrentPitchAngle);
	}
	///////////////////////////////////////////////////

	//Bullet
	{
		FVector spawnLocation = muzzleLocation + direction * 100;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, spawnLocation, direction.Rotation());
		bullet->SetPlayer(Cast<ACPlayer>(OwnerCharacter));
		bullet->Impact(start, end, OwnerCharacter);
		bullet->Shoot(direction);
	}

	//Crosshiar
	if (CurrSpreadRadius <= 1.0f)
	{
		CurrSpreadRadius += SpreadSpeed * GetWorld()->GetDeltaSeconds();
	}
	LastAddSpreadTime = GetWorld()->GetTimeSeconds();

	CurrMagazineCount--;
	Widget->SetCurrentMagazineCount(CurrMagazineCount);
	Widget->UpdateMagazineCount();
	if (CurrMagazineCount <= 0)
		Reload();
}

void ACRifle::Aiming(float FOV)
{
	Camera->FieldOfView = FOV;
}

void ACRifle::Begin_Aim()
{
	CheckFalse(IsAvaliableZoom());
	CheckTrue(bAiming);

	bAiming = true;

	OriginAimData.TargetArmLength = SpringArm->TargetArmLength;
	OriginAimData.SocketOffset = SpringArm->SocketOffset;
	OriginAimData.bEnableCameraLag = SpringArm->bEnableCameraLag;

	SpringArm->TargetArmLength = 80;
	SpringArm->SocketOffset = FVector(0, 55, 10); 

	SpringArm->bEnableCameraLag = false;

	Camera->FieldOfView = 65;
	Timeline.PlayFromStart();

	LimitPitchAngle = 0.05f;
}

void ACRifle::End_Aim()
{
	CheckFalse(IsAvaliableZoom());
	CheckFalse(bAiming);

	bAiming = false;

	SpringArm->TargetArmLength = 200;
	SpringArm->SocketOffset = FVector(0, 50, 15);
	SpringArm->bEnableCameraLag = OriginAimData.bEnableCameraLag;

	Camera->FieldOfView = 90;

	Timeline.ReverseFromEnd();

	LimitPitchAngle = 0.25f;
}

bool ACRifle::IsAvaliableZoom()
{
	return !!SpringArm && !!Camera;
}

void ACRifle::Reload()
{
	bReload = false;
	End_Aim();

	if(bEquip)
		OwnerCharacter->PlayAnimMontage(ReloadMontage, ReloadMontage_PlayRate);
}

void ACRifle::Eject_Magazine()
{
	if (MagazineBoneName.IsValid())
		SkeletalMesh->HideBoneByName(MagazineBoneName, EPhysBodyOp::PBO_None);

	CheckNull(MagazineClass);

	FTransform transform = SkeletalMesh->GetSocketTransform(MagazineBoneName);
	ACMagazine* magazine = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazine->Eject();
	magazine->SetLifeSpan(5);
	magazine->FinishSpawning(transform);

	bVisibleMagazine = false;
}

void ACRifle::Spawn_Magazine()
{
	CheckNull(MagazineClass);
	
	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass);
	Magazine->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), AttachedMagazineSocketName);

}

void ACRifle::Load_Magazine()
{
	if (!!Magazine)
		Magazine->Destroy();

	if (MagazineBoneName.IsValid())
		SkeletalMesh->UnHideBoneByName(MagazineBoneName);

	CurrMagazineCount = MaxMagazineCount;
	Widget->SetCurrentMagazineCount(MaxMagazineCount);
	Widget->UpdateMagazineCount();
	bVisibleMagazine = true;
}

void ACRifle::End_Reload()
{
	bReload = true;
}

void ACRifle::Equip()
{
	Super::Equip();

	Widget->SetVisibility(ESlateVisibility::Visible);
	bVisibleMagazine = true;
}

void ACRifle::Unequip()
{
	Super::Unequip();

	Widget->SetVisibility(ESlateVisibility::Hidden);
	bVisibleMagazine = false;
}

