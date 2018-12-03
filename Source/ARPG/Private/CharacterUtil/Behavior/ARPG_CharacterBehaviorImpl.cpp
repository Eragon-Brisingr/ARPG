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
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(Montage))
	{
		OnMontageBlendingOutStarted->Unbind();
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

UCBC_PlayStateMontage::UCBC_PlayStateMontage()
{
	BehaviorType = UCB_PlayStateMontage::StaticClass();
}

void UCB_PlayStateMontage::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	UARPG_ActorFunctionLibrary::MoveActorTo(Executer, FVector(Location.X, Location.Y, Executer->GetActorLocation().Z), Rotation);

	UAnimMontage* StartMontage = GetConfig()->StartMontage;
	CurrentMontage = StartMontage;
	Executer->PlayMontage(StartMontage);
	FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenStartMontageEnd, Executer);
	Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, StartMontage);
}

void UCB_PlayStateMontage::AbortBehavior(class ACharacterBase* Executer)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = Executer->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(CurrentMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	UAnimMontage* EndMontage = GetConfig()->EndMontage;
	Executer->PlayMontage(EndMontage);
	FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenEndMontageEnd, Executer);
	Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, EndMontage);
}

void UCB_PlayStateMontage::WhenStartMontageEnd(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (bInterrupted)
	{
		FinishExecute(false);
	}
	else
	{
		UAnimMontage* LoopMontage = GetConfig()->LoopMontage;
		CurrentMontage = LoopMontage;
		Executer->PlayMontage(LoopMontage);
		FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenLoopMontageEnd, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, LoopMontage);
	}
}

void UCB_PlayStateMontage::WhenLoopMontageEnd(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	if (bInterrupted)
	{
		FinishExecute(false);
	}
	else
	{
		UAnimMontage* LoopMontage = GetConfig()->LoopMontage;
		CurrentMontage = LoopMontage;
		Executer->PlayMontage(LoopMontage);
		FOnMontageBlendingOutStarted OnMontageEnded = FOnMontageBlendingOutStarted::CreateUObject(this, &UCB_PlayStateMontage::WhenLoopMontageEnd, Executer);
		Executer->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnMontageEnded, LoopMontage);
	}
}

void UCB_PlayStateMontage::WhenEndMontageEnd(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer)
{
	FinishAbort();
}

const UCBC_PlayStateMontage* UCB_PlayStateMontage::GetConfig() const
{
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_PlayStateMontage>();
}
