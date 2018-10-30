// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterAnimInstance.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Animation/AnimMetaData.h>
#include "ARPG_CharacterAnimType.h"
#include "ARPG_MovementComponent.h"
#include "CharacterBase.h"




void UARPG_CharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld())
	{
		return;
	}
#endif

	if (ACharacterBase* Character = Cast<ACharacterBase>(TryGetPawnOwner()))
	{
		Character->ARPG_MovementComponent->OnARPGMovementModeChanged.AddDynamic(this, &UARPG_CharacterAnimInstance::OnMovementModeChanged);
	}
}

void UARPG_CharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UARPG_CharacterAnimInstance::OnMovementModeChanged(class UARPG_MovementComponent* MovementComponent, EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (MovementComponent->IsFalling())
	{
		for (int32 InstanceIndex = MontageInstances.Num() - 1; InstanceIndex >= 0; InstanceIndex--)
		{
			FAnimMontageInstance* MontageInstance = MontageInstances[InstanceIndex];
			if (MontageInstance && MontageInstance->IsActive())
			{
				UAnimMontage* ActiveMontage = MontageInstance->Montage;
				if (!ActiveMontage->GetMetaData().ContainsByPredicate([](UAnimMetaData* E) {return E && E->IsA<UAMD_CanPlayWhenFalling>(); }))
				{
					Montage_Stop(0.5, ActiveMontage);
				}
			}
		}
	}
}
