// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_GameStateBase.h"
#include "ARPG_TimeManager.h"
#include "ARPG_EnvironmentManager.h"




AARPG_GameStateBase::AARPG_GameStateBase()
{
	EnvironmentManager = CreateDefaultSubobject<UARPG_EnvironmentManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, EnvironmentManager));

	TimeManager = CreateDefaultSubobject<UARPG_TimeManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, TimeManager));
}

class UXD_TimeManager* AARPG_GameStateBase::GetGameTimeManager_Implementation() const
{
	return TimeManager;
}

class UXD_EnvironmentManager* AARPG_GameStateBase::GetEnvironmentManager_Implementation() const
{
	return EnvironmentManager;
}
