// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualLogger.h"

/**
 * 
 */
ARPG_API DECLARE_LOG_CATEGORY_EXTERN(ARPG_Gameplay_Log, Log, All);

#define Gameplay_Display_LOG(FMT, ...) UE_LOG(ARPG_Gameplay_Log, Display, TEXT(FMT), ##__VA_ARGS__)
#define Gameplay_Warning_LOG(FMT, ...) UE_LOG(ARPG_Gameplay_Log, Warning, TEXT(FMT), ##__VA_ARGS__)
#define Gameplay_Error_Log(FMT, ...) UE_LOG(ARPG_Gameplay_Log, Error, TEXT(FMT), ##__VA_ARGS__)

#define Gameplay_Display_VLog(LogOwner, FMT, ...) UE_VLOG(LogOwner, ARPG_Gameplay_Log, Display, TEXT(FMT), ##__VA_ARGS__)
#define Gameplay_Warning_VLog(LogOwner, FMT, ...) UE_VLOG(LogOwner, ARPG_Gameplay_Log, Warning, TEXT(FMT), ##__VA_ARGS__)
#define Gameplay_Error_VLog(LogOwner, FMT, ...) UE_VLOG(LogOwner, ARPG_Gameplay_Log, Error, TEXT(FMT), ##__VA_ARGS__)

#define Gameplay_Display_BLog(LogOwner, FMT, ...) Gameplay_Display_LOG(FMT, ##__VA_ARGS__); Gameplay_Display_VLog(LogOwner, FMT, ##__VA_ARGS__)
#define Gameplay_Warning_BLog(LogOwner, FMT, ...) Gameplay_Warning_LOG(FMT, ##__VA_ARGS__); Gameplay_Warning_VLog(LogOwner, FMT, ##__VA_ARGS__)
#define Gameplay_Error_BLog(LogOwner, FMT, ...) Gameplay_Error_Log(FMT, ##__VA_ARGS__); Gameplay_Error_VLog(LogOwner, FMT, ##__VA_ARGS__)
