#include "CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "UserInterfaces/CWidget_Base.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/WeaponStructures.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	Weapon->SetOwner(this);
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;

	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_CyberpunkSamurai.SK_CyberpunkSamurai'");
	GetMesh()->SetSkeletalMesh(mesh);

	CHelpers::GetClass<UCWidget_Base>(&HUDClass, "WidgetBlueprint'/Game/UI/HUD_Base/WBP_HUD_Base_Player.WBP_HUD_Base_Player_C'");

	TSubclassOf<UCAnimInstance> animInstance;

	CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 60, 0);
	SpringArm->bEnableCameraLag = true;
	
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Status->DisableControlRotation();
	Status->SetSpeed(ESpeedType::Run);

	if (!!HUDClass)
	{
		HUD = CreateWidget<UCWidget_Base, APlayerController>(GetController<APlayerController>(), HUDClass);
		HUD->AddToViewport();
		HUD->SetVisibility(ESlateVisibility::Visible);
	}

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	MaxHP = Status->GetHealth();
	HUD->UpdateHP(MaxHP, MaxHP);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetZoomValue);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACPlayer::OffRun);

	/*
	Mouse_R/Trigger_L
	Mouse_L/Trigger_R
	K_Space/C_A
	K_C/C_B
	K_R/C_X
	K_G/C_Y
	*/
	PlayerInputComponent->BindAction("K_Space/C_A", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction_A);
	PlayerInputComponent->BindAction("K_C/C_B", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction_B);
	PlayerInputComponent->BindAction("K_R/C_X", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction_X);
	PlayerInputComponent->BindAction("K_G/C_Y", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction_Y);
	PlayerInputComponent->BindAction("Mouse_R/Trigger_L", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction_LT);
	PlayerInputComponent->BindAction("Mouse_R/Trigger_L", EInputEvent::IE_Released, this, &ACPlayer::OffAction_LT);
	PlayerInputComponent->BindAction("Mouse_L/Trigger_R", EInputEvent::IE_Pressed, this, &ACPlayer::OnAction_RT);
	PlayerInputComponent->BindAction("Mouse_L/Trigger_R", EInputEvent::IE_Released, this, &ACPlayer::OffAction_RT);


	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::SetFistWeaponType);
	PlayerInputComponent->BindAction("Rifle", EInputEvent::IE_Pressed, this, &ACPlayer::SetRifleWeaponType);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	AddControllerPitchInput(InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	AddControllerYawInput(InAxis);
}

void ACPlayer::OnRun()
{
	Status->SetSpeed(ESpeedType::Sprint);
}

void ACPlayer::OffRun()
{
	Status->SetSpeed(ESpeedType::Run);
}


void ACPlayer::OnAction_A()
{
	Weapon->DoAction_A();
}

void ACPlayer::OnAction_B()
{
	Weapon->DoAction_B();
}

void ACPlayer::OnAction_X()
{
	Weapon->DoAction_X();
}

void ACPlayer::OnAction_Y()
{
	Weapon->DoAction_Y();
}

void ACPlayer::OnAction_LT()
{
	Weapon->DoAction_LT();
}

void ACPlayer::OffAction_LT()
{
	Weapon->OffDoAction_LT();
}

void ACPlayer::OnAction_RT()
{
	Weapon->DoAction_RT();
}

void ACPlayer::OffAction_RT()
{
	Weapon->OffDoAction_RT();
}

void ACPlayer::OffAction()
{
	if(State->IsActionMode())
		State->SetQuitActionMode();
}

void ACPlayer::SetFistWeaponType()
{
	CheckFalse(State->IsIdleMode());
	Weapon->SetFistMode();
}

void ACPlayer::SetRifleWeaponType()
{
	CheckFalse(State->IsIdleMode());
	Weapon->SetRifleMode();
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead: Dead(); break;
	}
}

float ACPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Amount = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent *)&DamageEvent;

	State->SetHittedMode();

	return damage;
}

void ACPlayer::Hitted()
{
	Status->Damage(Damage.Amount);
	Damage.Amount = 0;

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		bDead = true;
		return;
	}

	HUD->UpdateHP(Status->GetHealth(), MaxHP);

	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		FHitData* data = Damage.Event->HitData;

		data->PlayMontage(this);
		data->PlayHitStop(GetWorld());
		data->PlaySoundCue(this);
		data->PlayEffect(GetWorld(), GetActorLocation());


		FVector start = GetActorLocation();
		FVector target = Damage.Character->GetActorLocation();
		FVector direction = target - start;
		direction.Normalize();

		LaunchCharacter(-direction * data->Launch, false, false);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;

	//TODO : End_Hitted ÄÚµå °¡Á®¿È
	End_Hitted();
}

void ACPlayer::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TakeDamageMontages->PlayDeadMode();
}

void ACPlayer::End_Hitted()
{
	CLog::Log("Call End_Hitted");
	State->SetIdleMode();
}

void ACPlayer::End_Dead()
{
	Destroy();
}  
