// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_PropertyOperator.h"
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
	void SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const override;
};

UCLASS(meta = (DisplayName = "MaxHealth"))
class ARPG_API UARPG_MaxHealth_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	float GetValue(const UObject* Owner) const override;

	void PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
};

UCLASS(meta = (DisplayName = "Stamina"))
class ARPG_API UARPG_Stamina_PropertyOperator : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const override;
};

UCLASS(meta = (DisplayName = "MaxStamina"))
class ARPG_API UARPG_MaxStamina_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

UCLASS(meta = (DisplayName = "StaminaRestoreSpeed"))
class ARPG_API UARPG_StaminaRestoreSpeed_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

UCLASS(meta = (DisplayName = "StaminaRestoreCoolDownTime"))
class ARPG_API UARPG_StaminaRestoreCoolDownTime_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

UCLASS(meta = (DisplayName = "MaxBearload"))
class ARPG_API UARPG_MaxBearload_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

UCLASS(meta = (DisplayName = "MaxEquipLoad"))
class ARPG_API UARPG_MaxEquipLoad_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

