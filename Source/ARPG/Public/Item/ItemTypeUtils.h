// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
