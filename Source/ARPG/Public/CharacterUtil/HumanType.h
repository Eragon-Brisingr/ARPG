// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HumanType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EUseWeaponState :uint8
{
	NoneWeapon_Default UMETA(DisplayName = "未使用武器"),
	UsingWeapon_Defalut UMETA(DisplayName = "使用武器_默认状态")
};
