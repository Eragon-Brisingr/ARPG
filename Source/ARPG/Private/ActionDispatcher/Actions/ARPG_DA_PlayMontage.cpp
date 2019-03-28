// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_PlayMontage.h"
#include "CharacterBase.h"

bool UARPG_DA_PlayMontage::CanActiveAction() const
{
	return Pawn.Get() != nullptr;
}

void UARPG_DA_PlayMontage::WhenActionActived()
{
	RegisterEntity(Pawn.Get());
	ACharacterBase* Character = Cast<ACharacterBase>(Pawn.Get());
	Character->PlayMontage(MontageToPlay);

	FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UARPG_DA_PlayMontage::WhenMontageBlendingOutStart);
	Character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, MontageToPlay);
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

void UARPG_DA_PlayMontage::WhenMontageBlendingOutStart(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted == false)
	{
		FinishAction();
		WhenPlayFinished.ExecuteIfBound();
	}
}
