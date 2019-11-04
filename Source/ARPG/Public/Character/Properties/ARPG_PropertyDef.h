// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include "ARPG_PropertyDef.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARPG_API FARPG_GameplayFloatPropertyBase
{
	GENERATED_BODY()
public:
};

USTRUCT(BlueprintType)
struct FARPG_FloatProperty_ModifyConfig
{
	GENERATED_BODY()
public:
	float Value;
	TSoftObjectPtr<const UObject> Instigator;
	FName DescribeTag;
};

USTRUCT(BlueprintType)
struct ARPG_API FARPG_FloatProperty : public FARPG_GameplayFloatPropertyBase
{
	GENERATED_BODY()
public:
	FARPG_FloatProperty() = default;
	FARPG_FloatProperty(float InitValue)
		:Base(InitValue), Final(InitValue)
	{}

private:
	UPROPERTY(SaveGame)
	float Base;

	UPROPERTY(NotReplicated)
	float Additive = 0.f;

	UPROPERTY(NotReplicated)
	float Multiple = 1.f;

	UPROPERTY()
	float Final;

	FORCEINLINE void UpdateFinalValue() { Final = (Base + Additive) * Multiple; }
public:
	float Value() const { return Final; }

	void PushAdditiveMultipleModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
	{
		Additive += ModifyConfig.Value;
		UpdateFinalValue();
	}
	void PopAdditiveMultipleModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
	{
		Additive -= ModifyConfig.Value;
		UpdateFinalValue();
	}
	void PushMultipleModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
	{
		Multiple += ModifyConfig.Value;
		UpdateFinalValue();
	}
	void PopMultipleModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
	{
		Multiple -= ModifyConfig.Value;
		UpdateFinalValue();
	}
};

UCLASS(const, BlueprintType, hidedropdown)
class ARPG_API UARPG_GameplayFloatPropertyOperatorBase : public UObject
{
	GENERATED_BODY()
public:
	virtual float GetValue(const UObject* Owner) const { unimplemented(); return 0.f; }
	virtual void SetValue(UObject* Owner, float InValue, const UObject* InInstigator) const { unimplemented(); }

	// TODO：添加Additive与Multipe行为
};

UCLASS(const, BlueprintType, hidedropdown)
class ARPG_API UARPG_GameplayFloatPropertyModifierBase : public UARPG_GameplayFloatPropertyOperatorBase
{
	GENERATED_BODY()
public:
	virtual void PushAdditiveMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
	virtual void PopAdditiveMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
	virtual void PushMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
	virtual void PopMultipleModifier(UObject* Owner, const FARPG_FloatProperty_ModifyConfig& ModifyConfig) { unimplemented(); }
};

UCLASS(const)
class ARPG_API UARPG_GameplayFloatPropertyOperatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "角色|属性", meta = (CompactNodeTitle = "GetValue"))
	static float GetValue(TSubclassOf<UARPG_GameplayFloatPropertyOperatorBase> PropertyOperator, const UObject* Owner) { return Owner && PropertyOperator ? PropertyOperator.GetDefaultObject()->GetValue(Owner) : 0.f; }
};