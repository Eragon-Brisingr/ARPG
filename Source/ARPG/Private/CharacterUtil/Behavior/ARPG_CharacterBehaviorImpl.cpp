// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorImpl.h"
#include "CharacterBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"


UCBC_PlayMontage::UCBC_PlayMontage()
{
	BehaviorType = UCB_PlayMontage::StaticClass();
}

void UCB_PlayMontage::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	UARPG_ActorFunctionLibrary::MoveActorTo(Executer, FVector(Location.X, Location.Y, Executer->GetActorLocation().Z), Rotation);

	float duration = Executer->PlayMontage(GetConfig()->Montage);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCB_PlayMontage::WhenMontageEnd, duration, false);
}

void UCB_PlayMontage::AbortBehavior(class ACharacterBase* Executer)
{
	Executer->StopMontage(GetConfig()->Montage);
	Executer->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

const class UCBC_PlayMontage* UCB_PlayMontage::GetConfig() const
{
	return Cast<const UCBC_PlayMontage>(Config);
}

void UCB_PlayMontage::WhenMontageEnd()
{
	FinishBehavior();
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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCB_Wait::FinishBehavior, duration, false);
	}
	else
	{
		FinishBehavior();
	}
}

void UCB_Wait::AbortBehavior(class ACharacterBase* Executer)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

const UCBC_Wait* UCB_Wait::GetConfig() const
{
	return Cast<UCBC_Wait>(Config);
}
