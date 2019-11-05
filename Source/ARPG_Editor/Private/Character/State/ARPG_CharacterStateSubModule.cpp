// Fill out your copyright notice in the Description page of Project Settings.

#include <ModuleManager.h>
#include "ARPG_CharacterStateSubModule.h"
#include "AssetTypeActions_CharacterState.h"

void FARPG_CharacterStateSubModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	{
		AssetTypeActions_CharacterState = MakeShareable(new FAssetTypeActions_CharacterState());
		AssetTools.RegisterAssetTypeActions(AssetTypeActions_ItemCore.ToSharedRef());
	}
}

void FARPG_CharacterStateSubModule::ShutdownModule()
{
	if (FAssetToolsModule* AssetToolsModule = FModuleManager::Get().GetModulePtr<FAssetToolsModule>("AssetTools"))
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();
		AssetTools.UnregisterAssetTypeActions(AssetTypeActions_ItemCore.ToSharedRef());
	}
}
