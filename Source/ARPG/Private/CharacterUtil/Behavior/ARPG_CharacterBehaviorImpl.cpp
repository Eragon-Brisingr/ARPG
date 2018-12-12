// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorImpl.h"
#include "CharacterBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"


UCBC_PlayMontage::UCBC_PlayMontage()
{
	BehaviorType = UCB_PlayMontage::StaticClass();
}

void UCB_PlayMontage::ExecuteBehavior(class ACharacterBase* Executer)
{
	UAnimMontage* Montage = GetConfig()->Montage;
	Executer->PlayMontage(Montage);
	FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayMontage::WhenMontageBlendingOutStart, Executer);
	Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, Montage);
}

void UCB_PlayMontage::AbortBehavior(class ACharacterBase* Executer)
{
	UAnimMontage* Montage = GetConfig()->Montage;
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	Executer->StopMontage(Montage);
	FinishAbort();
}

const class UCBC_PlayMontage* UCB_PlayMontage::GetConfig() const
{
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_PlayMontage>();
}

void UCB_PlayMontage::WhenMontageBlendingOutStart(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	FinishExecute(bInterrupted == false);
}

UCBC_Wait::UCBC_Wait()
{
	BehaviorType = UCB_Wait::StaticClass();
}

void UCB_Wait::ExecuteBehavior(class ACharacterBase* Executer)
{
	float WaitTime = GetConfig()->WaitTime;
	float RandomRange = GetConfig()->RandomRange;
	float duration = GetConfig()->WaitTime + FMath::FRand() * RandomRange - RandomRange / 2.f;
	if (duration > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UCB_Wait::FinishExecute, true), duration, false);
	}
	else
	{
		FinishExecute(true);
	}
}

void UCB_Wait::AbortBehavior(class ACharacterBase* Executer)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

const UCBC_Wait* UCB_Wait::GetConfig() const
{
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_Wait>();
}

UCBC_PlayStateMontageSimpleBase::UCBC_PlayStateMontageSimpleBase()
{
	BehaviorType = UCB_PlayStateMontageSimple::StaticClass();
}

UCBC_PlayStateMontageSimpleRandom::UCBC_PlayStateMontageSimpleRandom()
{
	StartMontages.Add(nullptr);
	LoopMontages.Add(nullptr);
	EndMontages.Add(nullptr);
}

UAnimMontage* UCBC_PlayStateMontageSimpleRandom::GetStartMontage() const
{
	if (StartMontages.Num() > 0)
	{
		return StartMontages[FMath::RandHelper(StartMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCBC_PlayStateMontageSimpleRandom::GetLoopMontage() const
{
	if (LoopMontages.Num() > 0)
	{
		return LoopMontages[FMath::RandHelper(LoopMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCBC_PlayStateMontageSimpleRandom::GetEndMontage() const
{
	if (EndMontages.Num() > 0)
	{
		return EndMontages[FMath::RandHelper(EndMontages.Num())];
	}
	return nullptr;
}

void UCB_PlayStateMontageBase::ExecuteBehavior(class ACharacterBase* Executer)
{
	if (UAnimMontage* StartMontage = GetStartMontage())
	{
		CurrentMontage = StartMontage;
		Executer->PlayMontage(StartMontage);
		FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontageBase::WhenStartMontageBlendingOutStarted, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, StartMontage);
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_PlayStateMontageBase::AbortBehavior(class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(CurrentMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	if (UAnimMontage* EndMontage = GetEndMontage())
	{
		Executer->PlayMontage(EndMontage);
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
			Executer->PlayMontage(LoopMontage);
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
			Executer->PlayMontage(LoopMontage);
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

const UCBC_PlayStateMontageSimpleBase* UCB_PlayStateMontageSimple::GetConfig() const
{
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_PlayStateMontageSimpleBase>();
}

UAnimMontage* UCB_PlayStateMontageSimple::GetStartMontage() const
{
	return GetConfig()->GetStartMontage();
}

UAnimMontage* UCB_PlayStateMontageSimple::GetLoopMontage() const
{
	return GetConfig()->GetLoopMontage();
}

UAnimMontage* UCB_PlayStateMontageSimple::GetEndMontage() const
{
	return GetConfig()->GetEndMontage();
}

UCBC_PlayStateMontageStandard::UCBC_PlayStateMontageStandard()
{
	BehaviorType = UCB_PlayStateMontageStandard::StaticClass();

	LoopConfig.Add(FPlayStateMontageStandardConfig());
}

const UCBC_PlayStateMontageStandard* UCB_PlayStateMontageStandard::GetConfig() const
{
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_PlayStateMontageStandard>();
}

UAnimMontage* UCB_PlayStateMontageStandard::GetStartMontage() const
{
	const TArray<UAnimMontage*>& StartMontages = GetConfig()->StartMontages;
	if (StartMontages.Num() > 0)
	{
		return StartMontages[FMath::RandHelper(StartMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageStandard::GetLoopMontage() const
{
	const TArray<FPlayStateMontageStandardConfig>& LoopConfig = GetConfig()->LoopConfig;
	if (LoopConfig.Num() > 0)
	{
		CurLoopRandomIndex = FMath::RandHelper(LoopConfig.Num());
		return LoopConfig[CurLoopRandomIndex].LoopMontage;
	}
	return nullptr;
}

UAnimMontage* UCB_PlayStateMontageStandard::GetEndMontage() const
{
	const TArray<FPlayStateMontageStandardConfig>& LoopConfig = GetConfig()->LoopConfig;
	if (CurLoopRandomIndex < LoopConfig.Num())
	{
		const TArray<UAnimMontage*>& EndMontages = LoopConfig[CurLoopRandomIndex].EndMontages;
		return EndMontages[FMath::RandHelper(EndMontages.Num())];
	}
	return nullptr;
}

UCBC_TurnTo::UCBC_TurnTo()
{
	BehaviorType = UCB_TurnTo::StaticClass();
}

void UCB_TurnTo::ExecuteBehavior(class ACharacterBase* Executer)
{
	Executer->TurnTo(GetConfig<UCBC_TurnTo>()->TargetWorldRotation);
}

UCBC_EnterReleaseState::UCBC_EnterReleaseState()
{
	BehaviorType = UCB_EnterReleaseState::StaticClass();
}

void UCB_EnterReleaseState::ExecuteBehavior(class ACharacterBase* Executer)
{
	Executer->EnterReleaseState(OnBehaviorFinished);
}
