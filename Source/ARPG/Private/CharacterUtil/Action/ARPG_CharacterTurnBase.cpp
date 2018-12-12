// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterTurnBase.h"
#include "CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

void UCB_CharacterTurnBase::TurnTo(ACharacterBase* Executer, const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterTurnFinished)
{
	CurrentTurnMontage = GetTurnMontage(Executer, TargetWorldRotation);
	if (CurrentTurnMontage)
	{
		Executer->PlayMontage(CurrentTurnMontage);
		if (OnCharacterTurnFinished.IsBound())
		{
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_CharacterTurnBase::WhenMontageBlendOutStart, OnCharacterTurnFinished);
			Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, CurrentTurnMontage);
		}
	}
	else
	{
		OnCharacterTurnFinished.ExecuteIfBound(false);
	}
}

void UCB_CharacterTurnBase::AbortTurnTo(ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& OnCharacterBehaviorAbortFinished)
{
	UARPG_CharacterBehaviorBase::AbortBehavior(Executer, OnCharacterBehaviorAbortFinished);
}

void UCB_CharacterTurnBase::WhenMontageBlendOutStart(UAnimMontage* Montage, bool bInterrupted, FOnCharacterBehaviorFinished OnCharacterTurnFinished)
{
	OnCharacterTurnFinished.ExecuteIfBound(bInterrupted == false);
}

UAnimMontage* UCB_CharacterTurnBase::GetTurnMontageFourDirection(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180)
{
	float Yaw = (CurrentWorldRotation - TargetWorldRotation).GetNormalized().Yaw;
	UAnimMontage* MontageToPlay = nullptr;
	if (Yaw > 135.f)
	{
		MontageToPlay = TurnLeft180;
	}
	else if (Yaw > 45.f)
	{
		MontageToPlay = TurnLeft90;
	}
	else if (Yaw > -45.f)
	{
		MontageToPlay = TurnRight90;
	}
	else if (Yaw > -135.f)
	{
		MontageToPlay = TurnRight180;
	}
	return MontageToPlay;
}

void UCB_CharacterTurnBase::AbortBehavior(ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(CurrentTurnMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	Executer->StopAnimMontage(CurrentTurnMontage);
	FinishAbort();
}

UAnimMontage* UARPG_CharacterTurnNormal::GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation)
{
	return GetTurnMontageFourDirection(Executer->GetActorRotation(), TargetWorldRotation, TurnLeft90, TurnRight90, TurnLeft180, TurnRight180);
}
