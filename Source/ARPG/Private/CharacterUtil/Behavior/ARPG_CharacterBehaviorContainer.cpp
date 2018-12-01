// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorContainer.h"

UCBC_RandomSelect::UCBC_RandomSelect()
{
	BehaviorType = UCB_RandomSelect::StaticClass();
}

void UCB_RandomSelect::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
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

void UCB_RandomSelect::AbortBehavior(class ACharacterBase* Executer)
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
	return Cast<UCBC_RandomSelect>(Config);
}
