// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG.h"
#include "Modules/ModuleManager.h"
#include "ShaderCore.h"

class FARPGGameModule : public FDefaultGameModuleImpl
{

public:
	void StartupModule() override
	{
		// Maps virtual shader
		FString ShaderDir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
		AddShaderSourceDirectoryMapping(TEXT("/ARPG"), ShaderDir);
	}


	void ShutdownModule() override
	{

	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FARPGGameModule, ARPG, "ARPG" );
