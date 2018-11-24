// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EDodgeDirection : uint8
{
	Forward,
	Backword,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EAlertState : uint8
{
	None,
	Alerting,
	AlertEntirely
};

//关系
UENUM(BlueprintType)
enum class ECharacterRelationship :uint8
{
	Hostile = 0 UMETA(DisplayName = "仇恨"),
	Hate = 1 UMETA(DisplayName = "讨厌"),
	Neutral = 2 UMETA(DisplayName = "中立"),
	Friendly = 3 UMETA(DisplayName = "友好"),
	Cordial = 4 UMETA(DisplayName = "热情")
};

