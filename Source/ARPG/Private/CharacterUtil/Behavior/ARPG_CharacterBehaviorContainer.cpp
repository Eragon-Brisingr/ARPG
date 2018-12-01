// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorContainer.h"

UCBC_RandomSelect::UCBC_RandomSelect()
{
	BehaviorType = UCB_RandomSelect::StaticClass();
}

void UCB_RandomSelect::ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation)
{

}

void UCB_RandomSelect::AbortBehavior(class ACharacterBase* Executer)
{

}

const UCBC_RandomSelect* UCB_RandomSelect::GetConfig() const
{
	return Cast<UCBC_RandomSelect>(Config);
}
