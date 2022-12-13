#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeapon.h"

UCAnimInstance::UCAnimInstance()
{
}

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	FRotator ownerRotation = UKismetMathLibrary::NormalizedDeltaRotator(OwnerCharacter->GetControlRotation(), OwnerCharacter->GetActorRotation());
	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

	FRotator current = FRotator(Pitch, 0, 0);

	FRotator A = OwnerCharacter->GetControlRotation();
	FRotator B = OwnerCharacter->GetActorRotation();
	FRotator target = UKismetMathLibrary::NormalizedDeltaRotator(A, B);
	
	Pitch = UKismetMathLibrary::RInterpTo(current, target, DeltaSeconds, 0).Pitch;
	Pitch = UKismetMathLibrary::ClampAngle(Pitch, -90, 90);

	FRotator forward = OwnerCharacter->GetControlRotation();
	if (!!Weapon && Weapon->IsRifleMode())
	{
		bAiming = Weapon->IsAiming();
		bFire = Weapon->IsFire();
		
		if (Weapon->GetCurrentWeapon()->IsEquip())
		{
			forward = OwnerCharacter->GetActorRotation();
		}

		LeftHandLocation = Weapon->GetLeftHandLocation();

		bUseIK = false;
	}

	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), forward);
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}