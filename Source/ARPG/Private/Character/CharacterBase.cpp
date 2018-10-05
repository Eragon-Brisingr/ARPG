// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "XD_CharacterMovementComponent.h"
#include "ARPG_MovementComponent.h"


// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ARPG_MovementComponent = CastChecked<UARPG_MovementComponent>(GetCharacterMovement());

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::ARPG_InputPressed(EARPG_InputType InputType)
{
	InputBuffer.InputPressed(InputType);
}

void ACharacterBase::ARPG_InputReleased(EARPG_InputType InputType)
{
	InputBuffer.InputReleased(InputType);
}

bool ACharacterBase::ARPG_InputIsPressed(int32 InputType) const
{
	return InputBuffer.IsPressed(InputType);
}

bool ACharacterBase::ARPG_InputIsReleased(int32 InputType) const
{
	return InputBuffer.IsReleased(InputType);
}

void ACharacterBase::InvokeChangeMoveGait(ECharacterGait Gait)
{
	if (ARPG_MovementComponent->CurrentGait != Gait)
	{
		InvokeChangeMoveGaitToServer_Implementation(Gait);
		InvokeChangeMoveGaitToServer(Gait);
	}
}

void ACharacterBase::InvokeChangeMoveGaitToServer_Implementation(const ECharacterGait& Gait)
{
	ARPG_MovementComponent->bInvokeSprint = (Gait == ECharacterGait::Sprinting);
	ARPG_MovementComponent->SetGait(Gait);
}

FVector ACharacterBase::GetTargetLocation_Implementation(const FName& CurLockSocketName) const
{
	return GetMesh()->GetSocketLocation(CurLockSocketName);
}

bool ACharacterBase::CanLockedOnTarget_Implementation(AController* Invoker, const FName& InvokeLockedSocketName) const
{
	bool TryLockAgain;
	if (CanLockingOnTarget_Implementation(Invoker, InvokeLockedSocketName, TryLockAgain))
	{
		if (CanLockedSocketNames.Contains(InvokeLockedSocketName))
		{
			return true;
		}
	}
	return false;
}

bool ACharacterBase::CanLockingOnTarget_Implementation(AController* Invoker, const FName& CurLockSocketName, bool& TryLockAgain) const
{
	if (APawn* Pawn = Invoker->GetPawn())
	{
		if ((Pawn->GetActorLocation() - GetActorLocation()).Size() > MaxLockingDistance)
		{
			TryLockAgain = false;
			return false;
		}

		return true;
	}
	return false;
}

float ACharacterBase::PlayMontage(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/, bool ClientMaster /*= false*/)
{
	if (MontageToPlay)
	{
		if (ClientMaster)
		{
			if (HasAuthority())
			{
				if (GetController() && GetController()->IsLocalController())
				{
					MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
				}
			}
			else
			{
				PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
				PlayMontageToServer(MontageToPlay, InPlayRate, StartSectionName);
			}
		}
		else if (HasAuthority())
		{
			MulticastPlayMontage(MontageToPlay, InPlayRate, StartSectionName);
		}
		return MontageToPlay->GetPlayLength();
	}
	return 0.f;
}

void ACharacterBase::MulticastPlayMontage_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
}

void ACharacterBase::MulticastPlayMontageSkipOwner_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	if ((GetController() && GetController()->IsLocalController()) == false)
	{
		PlayAnimMontage(MontageToPlay, InPlayRate, StartSectionName);
	}
}

void ACharacterBase::StopMontage_Implementation()
{
	StopAnimMontage();
}

void ACharacterBase::PlayMontageToServer_Implementation(UAnimMontage * MontageToPlay, float InPlayRate /*= 1.f*/, FName StartSectionName /*= NAME_None*/)
{
	MulticastPlayMontageSkipOwner(MontageToPlay, InPlayRate, StartSectionName);
}

