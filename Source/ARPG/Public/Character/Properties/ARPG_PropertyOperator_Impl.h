// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_PropertyDef.h"
#include "ARPG_PropertyOperator_Impl.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "Health"))
class ARPG_API UARPG_Health_PropertyOperator : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const override;
};

UCLASS(meta = (DisplayName = "MaxHealth"))
class ARPG_API UARPG_MaxHealth_PropertyOperator : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const override;
	void PushAdditiveMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PopAdditiveMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
};
