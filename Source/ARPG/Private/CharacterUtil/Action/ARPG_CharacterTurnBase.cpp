// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterTurnBase.h"
#include "CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

void UCA_CharacterTurnBase::TurnTo(ACharacterBase* Executer, const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterTurnFinished)
{
	CurrentTurnMontage = GetTurnMontage(Executer, TargetWorldRotation);
	if (CurrentTurnMontage)
	{
		Executer->PlayMontage(CurrentTurnMontage);
		if (OnCharacterTurnFinished.IsBound())
		{
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCA_CharacterTurnBase::WhenMontageBlendOutStart, OnCharacterTurnFinished);
			Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, CurrentTurnMontage);
		}
	}
	else
	{
		OnCharacterTurnFinished.ExecuteIfBound(false);
	}
}

void UCA_CharacterTurnBase::AbortTurnTo(ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& OnCharacterBehaviorAbortFinished)
{
	UARPG_CharacterBehaviorBase::AbortBehavior(Executer, OnCharacterBehaviorAbortFinished);
}

void UCA_CharacterTurnBase::WhenMontageBlendOutStart(UAnimMontage* Montage, bool bInterrupted, FOnCharacterBehaviorFinished OnCharacterTurnFinished)
{
	OnCharacterTurnFinished.ExecuteIfBound(bInterrupted == false);
}

UAnimMontage* UCA_CharacterTurnBase::GetTurnMontageFourDirection(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180)
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

void UCA_CharacterTurnBase::AbortBehavior(ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(CurrentTurnMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	Executer->StopAnimMontage(CurrentTurnMontage);
	FinishAbort();
}

UAnimMontage* UCA_TurnNormal::GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation)
{
	return GetTurnMontageFourDirection(Executer->GetActorRotation(), TargetWorldRotation, TurnLeft90, TurnRight90, TurnLeft180, TurnRight180);
}
