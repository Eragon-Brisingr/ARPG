// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_PropertyDef.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARPG_API FARPG_PropertyChangeContext
{
	GENERATED_BODY()
public:
	FARPG_PropertyChangeContext() = default;
	FARPG_PropertyChangeContext(const TSoftObjectPtr<UObject>& Causer, const TSoftObjectPtr<UObject>& Instigator)
		:Causer(Causer), Instigator(Instigator)
	{}

	// 执行修改的实例，例如是武器、BUFF等
	TSoftObjectPtr<UObject> Causer;
	// 修改的始作俑者，一般为角色
	TSoftObjectPtr<UObject> Instigator;
};

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
	FARPG_FloatProperty_ModifyConfig() = default;
	FARPG_FloatProperty_ModifyConfig(float Value, const FName& DescribeTag, const TSoftObjectPtr<UObject>& Causer, const TSoftObjectPtr<UObject>& Instigator)
		:Value(Value), DescribeTag(DescribeTag), ChangeContext(Causer, Instigator)
	{}

	float Value;
	FName DescribeTag;
	FARPG_PropertyChangeContext ChangeContext;
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

	UPROPERTY(VisibleAnywhere)
	float Final;

	FORCEINLINE void UpdateFinalValue() { Final = (Base + Additive) * Multiple; }
public:
	FORCEINLINE float Value() const { return Final; }

	void PushAdditiveModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
	{
		Additive += ModifyConfig.Value;
		UpdateFinalValue();
	}
	void PopAdditiveModifier(const FARPG_FloatProperty_ModifyConfig& ModifyConfig)
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

UENUM()
enum class EARPG_PropertyOperatorOperand : uint8
{
	Additive,
	Multiple,
	Set
};

UENUM()
enum class EARPG_PropertyModifierOperand : uint8
{
	Additive,
	Multiple
};
