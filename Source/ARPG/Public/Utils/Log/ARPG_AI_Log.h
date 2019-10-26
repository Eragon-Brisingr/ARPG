// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualLogger.h"

/**
 * 
 */
ARPG_API DECLARE_LOG_CATEGORY_EXTERN(ARPG_AI_Log, Log, All);

#define AI_Display_LOG(FMT, ...) UE_LOG(ARPG_AI_Log, Display, TEXT(FMT), ##__VA_ARGS__)
#define AI_Warning_LOG(FMT, ...) UE_LOG(ARPG_AI_Log, Warning, TEXT(FMT), ##__VA_ARGS__)
#define AI_Error_Log(FMT, ...) UE_LOG(ARPG_AI_Log, Error, TEXT(FMT), ##__VA_ARGS__)

#define AI_Display_VLog(LogOwner, FMT, ...) UE_VLOG(LogOwner, ARPG_AI_Log, Display, TEXT(FMT), ##__VA_ARGS__)
#define AI_Warning_VLog(LogOwner, FMT, ...) UE_VLOG(LogOwner, ARPG_AI_Log, Warning, TEXT(FMT), ##__VA_ARGS__)
#define AI_Error_VLog(LogOwner, FMT, ...) UE_VLOG(LogOwner, ARPG_AI_Log, Error, TEXT(FMT), ##__VA_ARGS__)
