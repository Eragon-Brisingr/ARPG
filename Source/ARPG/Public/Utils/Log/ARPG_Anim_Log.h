// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <LogMacros.h>

// #include "CoreMinimal.h"
// #include "UObject/NoExportTypes.h"
// #include "ItemUtility.generated.h"

/**
 * 
 */
ARPG_API DECLARE_LOG_CATEGORY_EXTERN(ARPG_Anim_Log, Log, All);

#define Anim_Display_LOG(FMT, ...) UE_LOG(ARPG_Anim_Log, Display, TEXT(FMT), ##__VA_ARGS__)
#define Anim_Warning_LOG(FMT, ...) UE_LOG(ARPG_Anim_Log, Warning, TEXT(FMT), ##__VA_ARGS__)
#define Anim_Error_Log(FMT, ...) UE_LOG(ARPG_Anim_Log, Error, TEXT(FMT), ##__VA_ARGS__)