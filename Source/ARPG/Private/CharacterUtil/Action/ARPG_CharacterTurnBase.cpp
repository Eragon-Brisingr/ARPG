// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterTurnBase.h"
#include "CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

void UARPG_CharacterTurnBase::TurnTo(ACharacterBase* Character, const FRotator& TargetWorldRotation, const FOnCharacterActionFinished& OnCharacterTurnFinished) const
{
	if (UAnimMontage* MontageToPlay = ReceiveGetTurnMontage(Character, TargetWorldRotation))
	{
		Character->PlayMontage(MontageToPlay);
		if (OnCharacterTurnFinished.IsBound())
		{
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UARPG_CharacterTurnBase::WhenMontageBlendOut, OnCharacterTurnFinished);
			Character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, MontageToPlay);
		}
	}
	else
	{
		OnCharacterTurnFinished.ExecuteIfBound(false);
	}
}

UAnimMontage* UARPG_CharacterTurnBase::GetTurnMontage(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180) const
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

void UARPG_CharacterTurnBase::WhenMontageBlendOut(UAnimMontage* Montage, bool bInterrupted, FOnCharacterActionFinished OnCharacterTurnFinished)
{
	OnCharacterTurnFinished.ExecuteIfBound(bInterrupted == false);
}

void UARPG_CharacterTurnNormal::TurnTo(ACharacterBase* Character, const FRotator& TargetWorldRotation, const FOnCharacterActionFinished& OnCharacterTurnFinished) const
{
	if (UAnimMontage* MontageToPlay = GetTurnMontage(Character->GetActorRotation(), TargetWorldRotation, TurnLeft90, TurnRight90, TurnLeft180, TurnRight180))
	{
		Character->PlayMontage(MontageToPlay);
		if (OnCharacterTurnFinished.IsBound())
		{
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UARPG_CharacterTurnBase::WhenMontageBlendOut, OnCharacterTurnFinished);
			Character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, MontageToPlay);
		}
	}
	else
	{
		OnCharacterTurnFinished.ExecuteIfBound(false);
	}
}
