// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_GameStateBase.h"
#include "ARPG_TimeManager.h"
#include "ARPG_EnvironmentManager.h"
#include "ARPG_CampManager.h"




AARPG_GameStateBase::AARPG_GameStateBase()
{
	EnvironmentManager = CreateDefaultSubobject<UARPG_EnvironmentManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, EnvironmentManager));

	TimeManager = CreateDefaultSubobject<UARPG_TimeManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, TimeManager));

	CampManager = CreateDefaultSubobject<UARPG_CampManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, CampManager));
}

class UXD_TimeManager* AARPG_GameStateBase::GetGameTimeManager_Implementation() const
{
	return TimeManager;
}

class UXD_EnvironmentManager* AARPG_GameStateBase::GetEnvironmentManager_Implementation() const
{
	return EnvironmentManager;
}

class UXD_CampManager* AARPG_GameStateBase::GetCampManager_Implementation() const
{
	return CampManager;
}
