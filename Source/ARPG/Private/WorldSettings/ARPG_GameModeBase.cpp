// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_GameModeBase.h"
#include "XD_SaveGameFunctionLibrary.h"




void AARPG_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UXD_SaveGameFunctionLibrary::InitAutoSaveLoadSystem(this);
}

void AARPG_GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UXD_SaveGameFunctionLibrary::ShutDownAutoSaveLoadSystem(this);
}
