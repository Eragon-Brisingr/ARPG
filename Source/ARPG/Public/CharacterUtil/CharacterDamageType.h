// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterDamageType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPhysicsDamageType : uint8
{
	Hit UMETA(DisplayName = "打击"),
	Slash UMETA(DisplayName = "斩击"),
	Stab UMETA(DisplayName = "突刺")
};
