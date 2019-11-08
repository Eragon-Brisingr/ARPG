﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include "ARPG_PropertyDef.h"
#include "ARPG_PropertyOperator.generated.h"

/**
 * 
 */

UCLASS(const, BlueprintType, hidedropdown)
class ARPG_API UARPG_GameplayFloatPropertyGetterBase : public UObject
{
	GENERATED_BODY()
public:
	virtual float GetValue(const UObject* Owner) const { unimplemented(); return 0.f; }
	virtual FText GetPropertyName() const { unimplemented(); return FText::GetEmpty(); }
};

UCLASS(const, BlueprintType, hidedropdown)
class ARPG_API UARPG_GameplayFloatPropertyOperatorBase : public UARPG_GameplayFloatPropertyGetterBase
{
	GENERATED_BODY()
public:
	virtual void SetValue(UObject* Owner, float InValue, const FARPG_PropertyChangeContext& ChangeContext) const { unimplemented(); }

	void AddValue(UObject* Owner, float Value, const FARPG_PropertyChangeContext& ChangeContext) { SetValue(Owner, GetValue(Owner) + Value, ChangeContext); }
	void MultipleValue(UObject* Owner, float Value, const FARPG_PropertyChangeContext& ChangeContext) { SetValue(Owner, GetValue(Owner) * Value, ChangeContext); }

	void ApplyValue(EARPG_PropertyOperatorOperand Operand, UObject* Owner, float Value, const FARPG_PropertyChangeContext& ChangeContext);
};

// 属性修改器维持了属性的状态，激活时添加修改，反激活后回退修改
UCLASS(const, BlueprintType, hidedropdown)
class ARPG_API UARPG_GameplayFloatPropertyModifierBase : public UARPG_GameplayFloatPropertyGetterBase
{
	GENERATED_BODY()
public:
	virtual void PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
	virtual void PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
	virtual void PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
	virtual void PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }

	// 这个函数不应该触发其它受影响的属性
	virtual void ChangePushedAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig);
	// 这个函数不应该触发其它受影响的属性
	virtual void ChangePushedMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig);

	void PushModifier(EARPG_PropertyModifierOperand Operand, UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig);
	void PopModifier(EARPG_PropertyModifierOperand Operand, UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig);
	void ChangePushedModifier(EARPG_PropertyModifierOperand Operand, UObject* Owner, const FARPG_FloatProperty_ModifyConfig& OldModifyConfig, const FARPG_FloatProperty_ModifyConfig& ChangeToModifyConfig);
};

UCLASS(const)
class ARPG_API UARPG_GameplayFloatPropertyOperatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "角色|属性", meta = (CompactNodeTitle = "GetValue"))
	static float GetValue(TSubclassOf<UARPG_GameplayFloatPropertyGetterBase> Getter, const UObject* Owner) { return Owner && Getter ? Getter.GetDefaultObject()->GetValue(Owner) : 0.f; }

	UFUNCTION(BlueprintPure, Category = "角色|属性", meta = (CompactNodeTitle = "GetName"))
	static FText GetPropertyName(TSubclassOf<UARPG_GameplayFloatPropertyGetterBase> Getter) { return Getter ? Getter.GetDefaultObject()->GetPropertyName() : FText::GetEmpty(); }
};

#define ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_DECLARE() \
float GetValue(const UObject* Owner) const override; \
void PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override; \
void PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override; \
void PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override; \
void PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) override;

#define ARPG_FLOAT_PPROPERTY_MODIFIER_CLASS_IMPL(ClassName, OwnerType, PropertyName) \
float ClassName##::GetValue(const UObject* Owner) const { return CastChecked<OwnerType>(Owner)->PropertyName.Value(); } \
void ClassName##::PushAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { CastChecked<OwnerType>(Owner)->PropertyName##_PushAdditiveModifier(ModifyConfig); } \
void ClassName##::PopAdditiveModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { CastChecked<OwnerType>(Owner)->PropertyName##_PopAdditiveModifier(ModifyConfig); } \
void ClassName##::PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { CastChecked<OwnerType>(Owner)->PropertyName##_PushMultipleModifier(ModifyConfig); } \
void ClassName##::PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { CastChecked<OwnerType>(Owner)->PropertyName##_PopMultipleModifier(ModifyConfig); }
