// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_EditorSettings.generated.h"

/**
 * 
 */
UCLASS(Config = "ARPG_EditorSetting", defaultconfig)
class ARPG_EDITOR_API UARPG_EditorSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "物品", Config)
	TSet<TSubclassOf<class AARPG_ItemBase>> AllowedCreateParentClasses;
};
