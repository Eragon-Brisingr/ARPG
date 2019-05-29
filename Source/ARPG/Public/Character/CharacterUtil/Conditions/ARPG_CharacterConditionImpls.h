// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterConditionBase.h"
#include "ARPG_CharacterConditionImpls.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "在地上"))
class ARPG_API UACC_OnLand : public UARPG_CharacterConditionBase
{
	GENERATED_BODY()
public:
	bool CalculateConditionValue_Implementation(class ACharacterBase* Character) const override;
};
