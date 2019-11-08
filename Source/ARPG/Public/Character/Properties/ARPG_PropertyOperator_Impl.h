// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_PropertyOperator.h"
#include "ARPG_PropertyOperator_Impl.generated.h"

/**
 * 
 */

// 当前生命
UCLASS(meta = (DisplayName = "Health"))
class ARPG_API UARPG_Health_PropertyOperator : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const override;
};

// 生命上限
UCLASS(meta = (DisplayName = "MaxHealth"))
class ARPG_API UARPG_MaxHealth_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	float GetValue(const UObject* Owner) const override;
	void PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;
	void PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;

	void ChangePushedAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig) override;
	void ChangePushedMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig) override;
};

// 当前精力
UCLASS(meta = (DisplayName = "Stamina"))
class ARPG_API UARPG_Stamina_PropertyOperator : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	float GetValue(const UObject* Owner) const override;
	void SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const override;
};

// 精力上限
UCLASS(meta = (DisplayName = "MaxStamina"))
class ARPG_API UARPG_MaxStamina_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();

	void ChangePushedAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig) override;
	void ChangePushedMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig) override;
};

// 精力恢复速度
UCLASS(meta = (DisplayName = "StaminaRestoreSpeed"))
class ARPG_API UARPG_StaminaRestoreSpeed_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 精力再恢复冷却时间
UCLASS(meta = (DisplayName = "StaminaRestoreCoolDownTime"))
class ARPG_API UARPG_StaminaRestoreCoolDownTime_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 冲刺精力消耗速度
UCLASS(meta = (DisplayName = "SprintStaminaReduceSpeed"))
class ARPG_API UARPG_SprintStaminaReduceSpeed_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 当前总计负重
UCLASS(meta = (DisplayName = "Bearload"))
class ARPG_API UARPG_Bearload_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 总计负重上限
UCLASS(meta = (DisplayName = "MaxBearload"))
class ARPG_API UARPG_MaxBearload_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 当前装备负重
UCLASS(meta = (DisplayName = "EquipLoad"))
class ARPG_API UARPG_EquipLoad_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 装备负重上限
UCLASS(meta = (DisplayName = "MaxEquipLoad"))
class ARPG_API UARPG_MaxEquipLoad_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};

// 削韧积累值
UCLASS(meta = (DisplayName = "Stamina"))
class ARPG_API UARPG_HitStunValue_PropertyOperator : public UARPG_GameplayFloatPropertyGetterBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	float GetValue(const UObject* Owner) const override;
};

// 强韧度
UCLASS(meta = (DisplayName = "Toughness"))
class ARPG_API UARPG_Toughness_PropertyModifier : public UARPG_GameplayFloatPropertyModifierBase
{
	GENERATED_BODY()
public:
	FText GetPropertyName() const override;

	ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE();
};
