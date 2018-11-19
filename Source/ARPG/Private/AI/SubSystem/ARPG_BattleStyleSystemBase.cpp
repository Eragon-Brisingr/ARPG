// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BattleStyleSystemBase.h"
#include "CharacterBase.h"

class UWorld* UARPG_BattleStyleSystemBase::GetWorld() const
{
	return Character->GetWorld();
}
