// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ItemType.h"
#include "ItemTypeUtils.generated.h"

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

USTRUCT(BlueprintType)
struct ARPG_API FARPG_Item : public FXD_Item
{
	GENERATED_BODY()
public:
	FARPG_Item();
};
