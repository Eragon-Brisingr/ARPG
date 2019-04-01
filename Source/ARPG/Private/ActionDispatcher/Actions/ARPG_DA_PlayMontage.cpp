// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_PlayMontage.h"
#include "CharacterBase.h"

bool UARPG_DA_PlayMontage::IsActionValid() const
{
	return Pawn.Get() != nullptr;
}

void UARPG_DA_PlayMontage::WhenActionActived()
{
	RegisterEntity(Pawn.Get());
	ACharacterBase* Character = Cast<ACharacterBase>(Pawn.Get());
	Character->PlayMontage(MontageToPlay);

	FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UARPG_DA_PlayMontage::WhenMontagePlayFinished);
	Character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, MontageToPlay);
}

void UARPG_DA_PlayMontage::WhenActionAborted()
{
	if (WhenAbortedMontage)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(Pawn.Get());
		Character->PlayMontage(WhenAbortedMontage);
		
		FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UARPG_DA_PlayMontage::WhenAbortPlayFinished);
		Character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, WhenAbortedMontage);
	}
	else
	{
		DeactiveAction();
	}
}

void UARPG_DA_PlayMontage::WhenActionDeactived()
{
	ACharacterBase* Character = Cast<ACharacterBase>(Pawn.Get());
	Character->StopMontage(MontageToPlay);
}

void UARPG_DA_PlayMontage::WhenActionFinished()
{
	UnregisterEntity(Pawn.Get());
}

void UARPG_DA_PlayMontage::WhenMontagePlayFinished(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == false)
	{
		FinishAction();
		WhenPlayFinished.ExecuteIfBound();
	}
	else
	{
		AbortDispatcher({});
	}
}

void UARPG_DA_PlayMontage::WhenAbortPlayFinished(UAnimMontage* Montage, bool bInterrupted)
{
	DeactiveAction();
}
