// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorContainer.h"

UCBC_RandomSelect::UCBC_RandomSelect()
{
	BehaviorType = UCB_RandomSelect::StaticClass();
}

void UCB_RandomSelect::ExecuteBehavior(ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	CurrentBehavior = nullptr;

	float TotalWeight = 0;
	for (const FRandomSelectBehaviorElement& Element : GetConfig()->RandomBehaviors)
	{
		TotalWeight += Element.Weight;
	}

	float RandomRes = FMath::FRand() * TotalWeight;
	for (const FRandomSelectBehaviorElement& Element : GetConfig()->RandomBehaviors)
	{
		RandomRes -= Element.Weight;
		if (RandomRes < 0.f)
		{
			CurrentBehavior = Element.Behavior;
			break;
		}
	}

	if (CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, Location, Rotation, OnBehaviorFinished);
	}
	else
	{
		FinishExecute(true);
	}
}

void UCB_RandomSelect::AbortBehavior(ACharacterBase* Executer)
{
	if (CurrentBehavior)
	{
		CurrentBehavior->AbortBehavior(Executer, OnBehaviorAbortFinished);
	}
	else
	{
		FinishAbort();
	}
}

const UCBC_RandomSelect* UCB_RandomSelect::GetConfig() const
{
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_RandomSelect>();
}

UCBC_Sequence::UCBC_Sequence()
{
	BehaviorType = UCB_Sequence::StaticClass();
}

void UCB_Sequence::ExecuteBehavior(ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	ExecuteIndex = 0;
	ExecuteElement(Executer, Location, Rotation);
}

void UCB_Sequence::AbortBehavior(ACharacterBase* Executer)
{
	if (ExecuteIndex < GetConfig()->Behaviors.Num())
	{
		if (UARPG_CharacterBehaviorConfigBase* Behavior = GetConfig()->Behaviors[ExecuteIndex])
		{
			Behavior->AbortBehavior(Executer, UARPG_CharacterBehaviorBase::FOnBehaviorAbortFinished::CreateUObject(this, &UCB_Sequence::WhenElementBehaviorAbortFinished));
			return;
		}
	}
	FinishAbort();
}

const UCBC_Sequence* UCB_Sequence::GetConfig() const
{
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_Sequence>();
}

void UCB_Sequence::WhenElementBehaviorFinished(bool Succeed, ACharacterBase* Executer, FVector Location, FRotator Rotation)
{
	if (Succeed)
	{
		ExecuteIndex += 1;
		ExecuteElement(Executer, Location, Rotation);
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_Sequence::WhenElementBehaviorAbortFinished()
{
	FinishAbort();
}

void UCB_Sequence::ExecuteElement(ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{
	if (ExecuteIndex < GetConfig()->Behaviors.Num())
	{
		if (UARPG_CharacterBehaviorConfigBase* Behavior = GetConfig()->Behaviors[ExecuteIndex])
		{
			Behavior->ExecuteBehavior(Executer, Location, Rotation, UARPG_CharacterBehaviorBase::FOnBehaviorFinished::CreateUObject(this, &UCB_Sequence::WhenElementBehaviorFinished, Executer, Location, Rotation));
		}
		else
		{
			FinishExecute(false);
		}
	}
	else
	{
		FinishExecute(true);
	}
}
