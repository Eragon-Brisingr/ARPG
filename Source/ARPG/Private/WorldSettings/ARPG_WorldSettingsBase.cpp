// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WorldSettingsBase.h"
#include "ARPG_TimeManager.h"

void AARPG_WorldSettingsBase::WhenGameInit_Implementation()
{
	UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(this);
	TimeManager->CurrentTime = StartGameTime;
	TimeManager->TimeSpendRate = TimeSpendRate;
}
