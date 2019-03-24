// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WorldSettingsBase.h"
#include "ARPG_TimeManager.h"
#include "Engine/World.h"

void AARPG_WorldSettingsBase::WhenGameInit_Implementation()
{
	if (IsMainWorldSettings())
	{
		UXD_TimeManager* TimeManager = UARPG_TimeManager::GetGameTimeManager(this);
		TimeManager->CurrentTime = StartGameTime;
		TimeManager->TimeSpendRate = TimeSpendRate;
	}
}

bool AARPG_WorldSettingsBase::NeedSave_Implementation() const
{
	return IsMainWorldSettings();
}

bool AARPG_WorldSettingsBase::IsMainWorldSettings() const
{
	return GetLevel() == GetWorld()->PersistentLevel;
}
