// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_GameStateBase.h"
#include "XD_EnvironmentManager.h"
#include "XD_TimeManager.h"




AARPG_GameStateBase::AARPG_GameStateBase()
{
	EnvironmentManager = CreateDefaultSubobject<UXD_EnvironmentManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, EnvironmentManager));

	TimeManager = CreateDefaultSubobject<UXD_TimeManager>(GET_MEMBER_NAME_CHECKED(AARPG_GameStateBase, TimeManager));
}
