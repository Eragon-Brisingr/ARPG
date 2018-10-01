// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_InputBuffer.generated.h"

/**
 * 
 */

namespace ARPG_InputBuffer
{
	UENUM()
	enum EARPG_InputBuffer
	{
		LeftLightAttack,
		LeftHeavyAttack,
		RightLightAttack,
		RightHeavyAttack,

		Forward,
		Left,
		Right,
		Backworld,

		Dodge,
		Down,
		Up
	};
}

USTRUCT(BlueprintType)
struct ARPG_API FARPG_InputBuffer
{
	GENERATED_BODY()




};