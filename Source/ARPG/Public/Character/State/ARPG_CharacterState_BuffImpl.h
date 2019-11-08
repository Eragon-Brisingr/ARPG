// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterStateBase.h"
#include "ARPG_PropertyDef.h"
#include "SubclassOf.h"
#include "ARPG_CharacterState_BuffImpl.generated.h"

class UARPG_GameplayFloatPropertyModifierBase;
class UARPG_GameplayFloatPropertyOperatorBase;

/**
 * 
 */
USTRUCT()
struct ARPG_API FGeneralBuffModifierConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	TSubclassOf<UARPG_GameplayFloatPropertyModifierBase> ModifyProperty;
	UPROPERTY(EditAnywhere, Category = "Buff")
	EARPG_PropertyModifierOperand Operand;
	UPROPERTY(EditAnywhere, Category = "Buff")
	float Value;
};

USTRUCT()
struct ARPG_API FGeneralBuffOperatorConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	TSubclassOf<UARPG_GameplayFloatPropertyOperatorBase> OperatorProperty;
	UPROPERTY(EditAnywhere, Category = "Buff")
	EARPG_PropertyOperatorOperand Operand;
	UPROPERTY(EditAnywhere, Category = "Buff")
	float Value;
};

UENUM()
enum class EGeneralBuffOverlayType : uint8
{
	// 用新的覆盖旧的效果
	Replace,
	// 延长持续时间
	ExtendTime
};

UCLASS(meta = (DisplayName = "通用Buff"))
class ARPG_API UARPG_CS_Buff_General : public UARPG_CharacterState_BuffBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "Buff名")
	FText BuffName;

	UPROPERTY(EditAnywhere, Category = "Buff", meta = (EditCondition = bAllowMulitSameTypeBuff, DisplayName = "覆盖方式"))
	EGeneralBuffOverlayType GeneralBuffOverlayType;

	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "起效的属性修改器")
	TArray<FGeneralBuffModifierConfig> EffectModifiers;

	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "激活时修改的属性")
	TArray<FGeneralBuffOperatorConfig> ActiveOperators;
	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "持续修改的属性")
	TArray<FGeneralBuffOperatorConfig> TickOperators;
	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "结束后修改的属性")
	TArray<FGeneralBuffOperatorConfig> DeactiveOperators;

	void WhenActived(bool IsFirstInit) override;
	void WhenTick(float DeltaTime) override;
	void WhenDeactived() override; 
	void WhenRepeatActive() override;
	FText GetStateName() const override { return BuffName; }
};
