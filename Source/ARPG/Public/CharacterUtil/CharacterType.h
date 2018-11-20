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
