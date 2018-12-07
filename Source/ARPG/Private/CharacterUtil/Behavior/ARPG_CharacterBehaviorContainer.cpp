// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorContainer.h"

UCBC_RandomSelect::UCBC_RandomSelect()
{
	BehaviorType = UCB_RandomSelect::StaticClass();
}

void UCB_RandomSelect::ExecuteBehavior(class ACharacterBase* Executer)
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
		CurrentBehavior->ExecuteBehavior(Executer, OnBehaviorFinished);
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

void UCB_Sequence::ExecuteBehavior(class ACharacterBase* Executer)
{
	ExecuteIndex = 0;
	ExecuteElement(Executer);
}

void UCB_Sequence::AbortBehavior(ACharacterBase* Executer)
{
	if (ExecuteIndex < GetConfig()->Behaviors.Num())
	{
		if (UARPG_CharacterBehaviorConfigBase* Behavior = GetConfig()->Behaviors[ExecuteIndex])
		{
			Behavior->AbortBehavior(Executer, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UCB_Sequence::WhenElementBehaviorAbortFinished));
			return;
		}
	}
	FinishAbort();
}

const UCBC_Sequence* UCB_Sequence::GetConfig() const
{
	return UARPG_CharacterBehaviorBase::GetConfig<UCBC_Sequence>();
}

void UCB_Sequence::WhenElementBehaviorFinished(bool Succeed, ACharacterBase* Executer)
{
	if (Succeed)
	{
		ExecuteIndex += 1;
		ExecuteElement(Executer);
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

void UCB_Sequence::ExecuteElement(ACharacterBase* Executer)
{
	if (ExecuteIndex < GetConfig()->Behaviors.Num())
	{
		if (UARPG_CharacterBehaviorConfigBase* Behavior = GetConfig()->Behaviors[ExecuteIndex])
		{
			Behavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_Sequence::WhenElementBehaviorFinished, Executer));
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
