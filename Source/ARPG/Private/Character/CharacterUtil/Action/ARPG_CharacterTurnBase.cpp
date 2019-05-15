// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterTurnBase.h"
#include "CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

void UCA_CharacterTurnBase::TurnTo(ACharacterBase* Executer, const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterTurnFinished)
{
	TurnToRotation = TargetWorldRotation;
	ExecuteBehavior(Executer, OnCharacterTurnFinished);
}

UAnimMontage* UCA_TurnMontageConfigBase::GetTurnMontageFourDirection(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180) const
{
	float Yaw = (CurrentWorldRotation - TargetWorldRotation).GetNormalized().Yaw;
	if (Yaw > 135.f)
	{
		return TurnLeft180;
	}
	else if (Yaw > 45.f)
	{
		return TurnLeft90;
	}
	else if (Yaw > -45.f)
	{
		return nullptr;
	}
	else if (Yaw > -135.f)
	{
		return TurnRight90;
	}
	else
	{
		return TurnRight180;
	}
}

UAnimMontage* UCA_TurnMontageConfigNormal::GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation) const
{
	return GetTurnMontageFourDirection(Executer->GetActorRotation(), TargetWorldRotation, TurnLeft90, TurnRight90, TurnLeft180, TurnRight180);
}

void UCA_CharacterTurnBase::AbortTurnTo(ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& OnCharacterBehaviorAbortFinished)
{
	UARPG_CharacterBehaviorBase::AbortBehavior(Executer, OnCharacterBehaviorAbortFinished);
}

void UCA_TurnByMontage::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	CurrentTurnMontage = TurnMontageConfig.GetDefaultObject()->GetTurnMontage(Executer, TurnToRotation);
	if (CurrentTurnMontage)
	{
		Executer->PlayMontageWithBlendingOutDelegate(CurrentTurnMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](UAnimMontage* Montage, bool bInterrupted)
			{
				FinishExecute(bInterrupted == false);
			}));
	}
	else
	{
		FinishExecute(true);
	}
}

void UCA_TurnByMontage::WhenBehaviorAborted(ACharacterBase* Executer)
{
	Executer->ClearMontageBlendingOutDelegate(CurrentTurnMontage);
	Executer->StopAnimMontage(CurrentTurnMontage);
	FinishAbort();
}

bool UCA_TurnByMontage::CanTurnTo(const ACharacterBase* Executer) const
{
	return TurnMontageConfig ? true : false;
}
