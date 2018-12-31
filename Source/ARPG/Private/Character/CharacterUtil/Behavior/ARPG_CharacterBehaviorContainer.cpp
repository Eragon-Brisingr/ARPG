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
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_RandomSelect>();
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
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_Sequence>();
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

UCBC_StateBehavior::UCBC_StateBehavior()
{
	BehaviorType = UCB_StateBehavior::StaticClass();
}

void UCB_StateBehavior::ExecuteBehavior(class ACharacterBase* Executer)
{
	if (CurrentBehavior = GetConfig()->StartBehavior, CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_StateBehavior::WhenStartBehaviorEnd, Executer));
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_StateBehavior::AbortBehavior(ACharacterBase* Executer)
{
	if (CurrentBehavior)
	{
		CurrentBehavior->AbortBehavior(Executer, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UCB_StateBehavior::WhenAbortBehaviorEnd, Executer));
	}
	else
	{
		FinishAbort();
	}
}

const UCBC_StateBehavior* UCB_StateBehavior::GetConfig() const
{
	return UARPG_CharacterBehaviorConfigurable::GetConfig<UCBC_StateBehavior>();
}

void UCB_StateBehavior::WhenStartBehaviorEnd(bool Succeed, ACharacterBase* Executer)
{
	if (CurrentBehavior = GetConfig()->LoopBehavior, CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_StateBehavior::WhenLoopBehaviorEnd, Executer));
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_StateBehavior::WhenLoopBehaviorEnd(bool Succeed, ACharacterBase* Executer)
{
	if (Succeed && CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_StateBehavior::WhenLoopBehaviorEnd, Executer));
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_StateBehavior::WhenEndBehaviorEnd(bool Succeed, ACharacterBase* Executer)
{
	CurrentBehavior = nullptr;
	FinishAbort();
}

void UCB_StateBehavior::WhenAbortBehaviorEnd(ACharacterBase* Executer)
{
	CurrentBehavior = GetConfig()->EndBehavior;
	if (CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_StateBehavior::WhenEndBehaviorEnd, Executer));
	}
	else
	{
		FinishAbort();
	}
}
