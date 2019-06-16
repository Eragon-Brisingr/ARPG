// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_PlayMontage.h"
#include "CharacterBase.h"

TArray<AActor*> UARPG_DA_PlayMontage::GetAllRegistableEntities() const
{
	return { Pawn.Get() };
}

bool UARPG_DA_PlayMontage::IsActionValid() const
{
	return Pawn.Get() != nullptr;
}

void UARPG_DA_PlayMontage::WhenActionActived()
{
	ACharacterBase* Character = Cast<ACharacterBase>(Pawn.Get());
	Character->PlayMontageWithBlendingOutDelegate(MontageToPlay, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](UAnimMontage* Montage, bool bInterrupted)
		{
			if (bInterrupted == false)
			{
				ExecuteEventAndFinishAction(WhenPlayFinished);
			}
			else
			{
				AbortDispatcher(true);
			}
		}));
}

void UARPG_DA_PlayMontage::WhenActionAborted()
{
	if (WhenAbortedMontage)
	{
		ACharacterBase* Character = Cast<ACharacterBase>(Pawn.Get());
		Character->PlayMontageWithBlendingOutDelegate(WhenAbortedMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=](UAnimMontage* Montage, bool bInterrupted)
			{
				DeactiveAction();
			}));
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
