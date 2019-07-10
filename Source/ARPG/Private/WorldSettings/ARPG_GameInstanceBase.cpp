// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_GameInstanceBase.h"
#include "SaveGame/ARPG_SaveGameSystem.h"




UARPG_GameInstanceBase::UARPG_GameInstanceBase()
{
	SaveGameSystem = CreateDefaultSubobject<UARPG_SaveGameSystem>(GET_MEMBER_NAME_CHECKED(UARPG_GameInstanceBase, SaveGameSystem));
}

class UXD_SaveGameSystemBase* UARPG_GameInstanceBase::GetSaveGameSystem_Implementation() const
{
	return SaveGameSystem;
}
