// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTypeUtils.generated.h"

class UARPG_ItemCoreBase;

/**
 * 
 */
UENUM(BlueprintType)
enum class ETradePart : uint8
{
	Buyer,
	Seller
};

UENUM(BlueprintType)
enum class EUseItemInput :uint8
{
	LeftMouse,
	RightMouse
};
