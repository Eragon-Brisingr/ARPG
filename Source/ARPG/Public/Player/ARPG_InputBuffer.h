// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_InputBuffer.generated.h"

/**
 * 
 */

UENUM(BlueprintType, meta = (Bitflags))
enum class EARPG_InputType : uint8
{
	LeftLightAttack,
	LeftHeavyAttack,
	RightLightAttack,
	RightHeavyAttack,

	Forward,
	Left,
	Right,
	Backward,
	Down,
	Up,

	Dodge,
	Sprint
};
ENUM_CLASS_FLAGS(EARPG_InputType);

USTRUCT(BlueprintType)
struct ARPG_API FARPG_InputBuffer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "EARPG_InputType"), Category = "输入")
	int32 Data;

	void InputPressed(EARPG_InputType InputType);

	void InputReleased(EARPG_InputType InputType);

	bool IsPressed(int32 InputType) const;

	bool IsReleased(int32 InputType) const;
};