// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_TimeManager.h"




UARPG_TimeManager* UARPG_TimeManager::GetARPG_TimeManager(const UObject* WorldContextObject)
{
	return CastChecked<UARPG_TimeManager>(GetGameTimeManager(WorldContextObject));
}
