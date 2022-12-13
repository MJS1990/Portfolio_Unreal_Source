#include "Weapons/WeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Sound/SoundCue.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(InOwner);
	if (!!state)
		state->SetActionMode();
	
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InOwner);

	if (!!status && bCanMove == false)
		status->SetStop();

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRatio);

	if (bFixedCamera == true)
		status->EnableControlRotation();
	else if (bFixedCamera == false)
		status->DisableControlRotation();
}

void FDoActionData::StopMontage(class ACharacter* InOwner)
{
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InOwner);

	if (!!status)
		bCanMove ? status->SetMove() : status->SetStop();

	if (!!Montage)
		InOwner->StopAnimMontage(Montage);
}

void FDoActionData::PlayEffect(ACharacter* InOwner)
{
	CheckNull(Effect);

	FVector location = EffectLocation;
	FVector scale = EffectScale;

	FVector ownerLocation = InOwner->GetActorLocation();
	ownerLocation += InOwner->GetActorRotation().RotateVector(location);

	FTransform transform;
	transform.SetLocation(ownerLocation);
	transform.SetRotation(FQuat(InOwner->GetActorRotation()));
	transform.SetScale3D(scale);

	CHelpers::PlayEffect(InOwner->GetWorld(), Effect, transform);
}

void FDoActionData::PlayEffect(USkeletalMeshComponent* InMesh, FName InSocketName)
{
	CheckNull(Effect);

	AActor* owner = InMesh->GetOwner();
	UWorld* world = owner->GetWorld();

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(world, Effect, transform, InMesh, InSocketName);
}

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(InOwner);

	if (!!status)
		bCanMove ? status->SetMove() : status->SetStop();

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRatio);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	if (FMath::IsNearlyZero(StopTime) == true) return;

	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);
		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;
			characters.Add(character);
		}
	}

	FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([=]()
	{
		for (ACharacter* character : characters)
			character->CustomTimeDilation = 1;
	});

	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::PlaySoundCue(ACharacter* InOwner)
{
	CheckNull(SoundCue);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, SoundCue, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);
}