// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "CoreMinimal.h"
// #include "UObject/NoExportTypes.h"
// #include "ARPG_LogCategorys.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(ARPG_Battle_Log, Log, All);

#define Battle_Display_LOG(FMT, ...) UE_LOG(ARPG_Battle_Log, Display, TEXT(FMT), ##__VA_ARGS__)
#define Battle_Warning_LOG(FMT, ...) UE_LOG(ARPG_Battle_Log, Warning, TEXT(FMT), ##__VA_ARGS__)
#define Battle_Error_Log(FMT, ...) UE_LOG(ARPG_Battle_Log, Error, TEXT(FMT), ##__VA_ARGS__)
