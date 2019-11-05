// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FAssetTypeActions_CharacterState;

struct FARPG_CharacterStateSubModule
{
	void StartupModule();
	void ShutdownModule();

private:
	TSharedPtr<FAssetTypeActions_CharacterState> AssetTypeActions_CharacterState;
};
