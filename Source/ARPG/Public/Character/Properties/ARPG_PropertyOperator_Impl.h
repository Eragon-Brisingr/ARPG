// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_PropertyDef.h"
#include "ARPG_PropertyOperator_Impl.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_Health_PropertyOperator : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const override;
};

UCLASS()
class ARPG_API UARPG_MaxHealth_PropertyOperator : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const override;
};
