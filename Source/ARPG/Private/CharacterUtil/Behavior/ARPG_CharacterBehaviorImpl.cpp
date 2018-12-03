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

void UCB_PlayMontage::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	UARPG_ActorFunctionLibrary::MoveActorTo(Executer, FVector(Location.X, Location.Y, Executer->GetActorLocation().Z), Rotation);

	UAnimMontage* Montage = GetConfig()->Montage;
	Executer->PlayMontage(Montage);
	FOnMontageEnded OnMontageEnded = FOnMontageEnded::CreateUObject(this, &UCB_PlayMontage::WhenMontageEnd);
	Executer->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnMontageEnded, Montage);
}

void UCB_PlayMontage::AbortBehavior(class ACharacterBase* Executer)
{
	UAnimMontage* Montage = GetConfig()->Montage;
	if (FAnimMontageInstance* MontageInstance = Executer->GetMesh()->GetAnimInstance()->GetActiveInstanceForMontage(Montage))
	{
		MontageInstance->OnMontageEnded.Unbind();
	}
	Executer->StopMontage(Montage);
}

const class UCBC_PlayMontage* UCB_PlayMontage::GetConfig() const
{
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_PlayMontage>();
}

void UCB_PlayMontage::WhenMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	FinishExecute(bInterrupted);
}

UCBC_Wait::UCBC_Wait()
{
	BehaviorType = UCB_Wait::StaticClass();
}

void UCB_Wait::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	UARPG_ActorFunctionLibrary::MoveActorTo(Executer, FVector(Location.X, Location.Y, Executer->GetActorLocation().Z), Rotation);
	
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
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_Wait>();
}

UCBC_PlayStateMontageBase::UCBC_PlayStateMontageBase()
{
	BehaviorType = UCB_PlayStateMontage::StaticClass();
}

UCBC_PlayStateMontageRandom::UCBC_PlayStateMontageRandom()
{
	StartMontages.Add(nullptr);
	LoopMontages.Add(nullptr);
	EndMontages.Add(nullptr);
}

UAnimMontage* UCBC_PlayStateMontageRandom::GetStartMontage() const
{
	if (StartMontages.Num() > 0)
	{
		return StartMontages[FMath::RandHelper(StartMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCBC_PlayStateMontageRandom::GetLoopMontage() const
{
	if (LoopMontages.Num() > 0)
	{
		return LoopMontages[FMath::RandHelper(LoopMontages.Num())];
	}
	return nullptr;
}

UAnimMontage* UCBC_PlayStateMontageRandom::GetEndMontage() const
{
	if (EndMontages.Num() > 0)
	{
		return EndMontages[FMath::RandHelper(EndMontages.Num())];
	}
	return nullptr;
}

void UCB_PlayStateMontage::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	UARPG_ActorFunctionLibrary::MoveActorTo(Executer, FVector(Location.X, Location.Y, Executer->GetActorLocation().Z), Rotation);

	if (UAnimMontage* StartMontage = GetConfig()->GetStartMontage())
	{
		CurrentMontage = StartMontage;
		Executer->PlayMontage(StartMontage);
		FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenStartMontageBlendingOutStarted, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, StartMontage);
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_PlayStateMontage::AbortBehavior(class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(CurrentMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	if (UAnimMontage* EndMontage = GetConfig()->GetEndMontage())
	{
		Executer->PlayMontage(EndMontage);
		FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenEndMontageBlendingOutStarted, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, EndMontage);
	}
	else
	{
		FinishAbort();
	}
}

void UCB_PlayStateMontage::WhenStartMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	if (bInterrupted)
	{
		FinishExecute(false);
	}
	else
	{
		if (UAnimMontage* LoopMontage = GetConfig()->GetLoopMontage())
		{
			CurrentMontage = LoopMontage;
			Executer->PlayMontage(LoopMontage);
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenLoopMontageBlendingOutStarted, Executer);
			Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, LoopMontage);
		}
		else
		{
			FinishExecute(false);
		}
	}
}

void UCB_PlayStateMontage::WhenLoopMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	if (bInterrupted)
	{
		FinishExecute(false);
	}
	else
	{
		if (UAnimMontage* LoopMontage = GetConfig()->GetLoopMontage())
		{
			CurrentMontage = LoopMontage;
			Executer->PlayMontage(LoopMontage);
			FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenLoopMontageBlendingOutStarted, Executer);
			Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, LoopMontage);
		}
		else
		{
			FinishExecute(false);
		}
	}
}

void UCB_PlayStateMontage::WhenEndMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	FinishAbort();
}

const UCBC_PlayStateMontageBase* UCB_PlayStateMontage::GetConfig() const
{
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_PlayStateMontageBase>();
}
