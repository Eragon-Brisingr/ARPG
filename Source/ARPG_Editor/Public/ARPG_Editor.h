// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"


class FARPG_EditorModule : public IModuleInterface
{
public:
	static uint32 ARPG_AssetCategory;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

