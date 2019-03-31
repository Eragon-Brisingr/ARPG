// Fill out your copyright notice in the Description page of Project Settings.

#include "Human_EnterReleaseState.h"
#include "HumanBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

void UHuman_EnterReleaseState::WhenBehaviorExecuted(ACharacterBase* Executer)
{
	if (AHumanBase* Human = Cast<AHumanBase>(Executer))
	{
		if (Human->UseWeaponState != EUseWeaponState::NoneWeapon_Default)
		{
			CurMontage = Human->TakeBackWeaponMontage;
			Human->PlayMontageWithBlendingOutDelegate(CurMontage, FOnMontageBlendingOutStarted::CreateUObject(this, &UHuman_EnterReleaseState::WhenMontageBlendOutStart));
		}
		else
		{
			FinishExecute(true);
		}
	}
	FinishExecute(false);
}

void UHuman_EnterReleaseState::WhenBehaviorAborted(class ACharacterBase* Executer)
{
	Executer->ClearMontageBlendingOutDelegate(CurMontage);
	FinishAbort();
}

void UHuman_EnterReleaseState::WhenMontageBlendOutStart(UAnimMontage* Montage, bool bInterrupted)
{
	FinishExecute(bInterrupted == false);
}
