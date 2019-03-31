// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorContainer.h"

void UCB_RandomSelect::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	CurrentBehavior = nullptr;

	float TotalWeight = 0;
	for (const FRandomSelectBehaviorElement& Element : RandomBehaviors)
	{
		TotalWeight += Element.Weight;
	}

	float RandomRes = FMath::FRand() * TotalWeight;
	for (const FRandomSelectBehaviorElement& Element : RandomBehaviors)
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

void UCB_RandomSelect::WhenBehaviorAborted(ACharacterBase* Executer)
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

void UCB_Sequence::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	ExecuteIndex = 0;
	ExecuteElement(Executer);
}

void UCB_Sequence::WhenBehaviorAborted(ACharacterBase* Executer)
{
	if (ExecuteIndex < Behaviors.Num())
	{
		if (UARPG_CharacterBehaviorBase* Behavior = Behaviors[ExecuteIndex])
		{
			Behavior->AbortBehavior(Executer, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UCB_Sequence::WhenElementBehaviorAbortFinished));
			return;
		}
	}
	FinishAbort();
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
	if (ExecuteIndex < Behaviors.Num())
	{
		if (UARPG_CharacterBehaviorBase* Behavior = Behaviors[ExecuteIndex])
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

void UCB_StateBehavior::WhenBehaviorExecuted(class ACharacterBase* Executer)
{
	CurrentBehavior = StartBehavior;
	if (CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_StateBehavior::WhenStartBehaviorEnd, Executer));
	}
	else
	{
		FinishExecute(false);
	}
}

void UCB_StateBehavior::WhenBehaviorAborted(ACharacterBase* Executer)
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

void UCB_StateBehavior::WhenStartBehaviorEnd(bool Succeed, ACharacterBase* Executer)
{
	CurrentBehavior = LoopBehavior;
	if (CurrentBehavior)
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
	CurrentBehavior = EndBehavior;
	if (CurrentBehavior)
	{
		CurrentBehavior->ExecuteBehavior(Executer, FOnCharacterBehaviorFinished::CreateUObject(this, &UCB_StateBehavior::WhenEndBehaviorEnd, Executer));
	}
	else
	{
		FinishAbort();
	}
}
