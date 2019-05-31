// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorImpl.h"
#include "CharacterBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

void UCB_PlayMontage::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	Executer->PlayMontage(MontageToPlay, {});
	FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayMontage::WhenMontageBlendingOutStart, Executer);
	Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, MontageToPlay);
}

void UCB_PlayMontage::WhenBehaviorAborted(class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(MontageToPlay))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	Executer->StopMontage(MontageToPlay);
	FinishAbort();
}

void UCB_PlayMontage::WhenMontageBlendingOutStart(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	FinishExecute(bInterrupted == false);
}

void UCB_Wait::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	float duration = WaitTime + FMath::FRand() * RandomRange - RandomRange / 2.f;
	if (duration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UCB_Wait::FinishExecute, true), duration, false);
	}
	else
	{
		FinishExecute(true);
	}
}

void UCB_Wait::WhenBehaviorAborted(class ACharacterBase* Executer)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UCB_PlayStateMontageBase::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	if (UAnimMontage* StartMontage = GetStartMontage())
	{
		CurrentMontage = StartMontage;
		Executer->PlayMontage(StartMontage, {});
		FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontageBase::WhenStartMontageBlendingOutStarted, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, StartMontage);
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_PlayStateMontageBase::WhenBehaviorAborted(class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(CurrentMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	if (UAnimMontage* EndMontage = GetEndMontage())
	{
		Executer->PlayMontage(EndMontage, {});
		FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontageBase::WhenEndMontageBlendingOutStarted, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, EndMontage);
	}
	else
	{
		FinishAbort();
	}
}

void UCB_PlayStateMontageBase::WhenStartMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (bInterrupted)
	{
		FinishExecute(false);
	}
	else
	{
		if (UAnimMontage* LoopMontage = GetLoopMontage())
		{
			CurrentMontage = LoopMontage;
			Executer->PlayMontage(LoopMontage, {});
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontageBase::WhenLoopMontageBlendingOutStarted, Executer);
			Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, LoopMontage);
		}
		else
		{
			FinishExecute(false);
		}
	}
}

void UCB_PlayStateMontageBase::WhenLoopMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (bInterrupted)
	{
		FinishExecute(false);
	}
	else
	{
		if (UAnimMontage* LoopMontage = GetLoopMontage())
		{
			CurrentMontage = LoopMontage;
			Executer->PlayMontage(LoopMontage, {});
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontageBase::WhenLoopMontageBlendingOutStarted, Executer);
			Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, LoopMontage);
		}
		else
		{
			FinishExecute(false);
		}
	}
}

void UCB_PlayStateMontageBase::WhenEndMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	FinishAbort();
}

UAnimMontage* UCB_PlayStateMontageSimple::GetStartMontage() const
{
	return StartMontage;
}

UAnimMontage* UCB_PlayStateMontageSimple::GetLoopMontage() const
{
	return LoopMontage;
}

UAnimMontage* UCB_PlayStateMontageSimple::GetEndMontage() const
{
	return EndMontage;
}

UCB_PlayStateMontageSimpleRandom::UCB_PlayStateMontageSimpleRandom()
{
	StartMontages.Add(nullptr);
	LoopMontages.Add(nullptr);
	EndMontages.Add(nullptr);
}

UAnimMontage* UCB_PlayStateMontageSimpleRandom::GetStartMontage() const
{
	if (StartMontages.Num() > 0)
	{
		return StartMontages[FMath::RandHelper(StartMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageSimpleRandom::GetLoopMontage() const
{
	if (LoopMontages.Num() > 0)
	{
		return LoopMontages[FMath::RandHelper(LoopMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageSimpleRandom::GetEndMontage() const
{
	if (EndMontages.Num() > 0)
	{
		return EndMontages[FMath::RandHelper(EndMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageStandard::GetStartMontage() const
{
	if (StartMontages.Num() > 0)
	{
		return StartMontages[FMath::RandHelper(StartMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageStandard::GetLoopMontage() const
{
	if (LoopConfig.Num() > 0)
	{
		CurLoopRandomIndex = FMath::RandHelper(LoopConfig.Num());
		return LoopConfig[CurLoopRandomIndex].LoopMontage;
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageStandard::GetEndMontage() const
{
	if (CurLoopRandomIndex < LoopConfig.Num())
	{
		const TArray<UAnimMontage*>& EndMontages = LoopConfig[CurLoopRandomIndex].EndMontages;
		return EndMontages[FMath::RandHelper(EndMontages.Num())];
	}
	return nullptr;
}

void UCB_TurnTo::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	Executer->TurnTo(TargetWorldRotation);
}

void UCB_EnterReleaseState::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	Executer->EnterReleaseState(OnBehaviorFinished);
}
