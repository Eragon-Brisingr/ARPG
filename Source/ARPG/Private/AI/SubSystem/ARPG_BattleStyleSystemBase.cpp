// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BattleStyleSystemBase.h"
#include "CharacterBase.h"
#include "AIController.h"

class UWorld* UARPG_BattleStyleSystemBase::GetWorld() const
{
	return Character->GetWorld();
}

class UAIPerceptionComponent* UARPG_BattleStyleSystemBase::GetPerceptionComponent() const
{
	if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
	{
		return AIController->GetAIPerceptionComponent();
	}
	return nullptr;
}
