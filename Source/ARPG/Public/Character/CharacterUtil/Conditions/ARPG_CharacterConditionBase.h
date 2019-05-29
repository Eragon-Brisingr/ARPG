// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_CharacterConditionBase.generated.h"

/**
 * 
 */
UCLASS(const, Abstract, Blueprintable, BlueprintType, hidedropdown)
class ARPG_API UARPG_CharacterConditionBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Ìõ¼þ")
	bool CalculateConditionValue(class ACharacterBase* Character) const;
	virtual bool CalculateConditionValue_Implementation(class ACharacterBase* Character) const { return false; }
};
