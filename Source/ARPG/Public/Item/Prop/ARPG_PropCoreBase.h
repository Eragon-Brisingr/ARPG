// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_PropertyDef.h"
#include "ARPG_CharacterStateBase.h"
#include "ARPG_PropCoreBase.generated.h"

class UARPG_CharacterState_BuffBase;
class UARPG_GameplayFloatPropertyOperatorBase;
class UARPG_GameplayFloatPropertyModifierBase;

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "道具"))
class ARPG_API UARPG_PropCoreBase : public UARPG_ItemCoreBase
{
	GENERATED_BODY()
public:
	FText GetItemTypeDesc() const override;
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
};

USTRUCT()
struct ARPG_API FGeneralPropOperatorConfig
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
enum class EPropSpecialBuffOverlayType : uint8
{
	// 无法叠加
	CanNotOverlap,
	// 用新的覆盖旧的效果
	Replace,
	// 混合，延长持续时间
	MixExtend,
	// 混合，使用当前最久的时间
	MixUseMaxTime
};

UENUM()
enum class EPropBuffOperatorOperand : uint8
{
	Additive
};

USTRUCT()
struct ARPG_API FPropBuffModifierConfig
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
struct ARPG_API FPropBuffOperatorConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	TSubclassOf<UARPG_GameplayFloatPropertyOperatorBase> OperatorProperty;
	UPROPERTY(EditAnywhere, Category = "Buff")
	EPropBuffOperatorOperand Operand;
	UPROPERTY(EditAnywhere, Category = "Buff")
	float Value;
};

UCLASS()
class ARPG_API UARPG_CS_PropBuff_Modifier : public UARPG_CharacterState_BuffBase
{
	GENERATED_BODY()
public:
	UARPG_CS_PropBuff_Modifier()
	{
		bAllowMulitSameTypeBuff = true;
		IntervalTime = -1.f;
	}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void WhenActived(bool IsFirstInit) override;
	void WhenDeactived() override;
	FText GetStateName() const override;

	UPROPERTY(SaveGame, Replicated)
	TSubclassOf<UARPG_GameplayFloatPropertyModifierBase> EffectProperty;

	UPROPERTY(SaveGame, Replicated)
	float AdditiveValue;
	UPROPERTY(SaveGame, Replicated)
	float MultipleAdditiveValue;
public:
	void SetAdditiveValueAfterActived(float Value);
	void SetMultipleValueAfterActived(float Value);
};

UCLASS()
class ARPG_API UARPG_CS_PropBuff_Operator : public UARPG_CharacterState_BuffBase
{
	GENERATED_BODY()
public:
	UARPG_CS_PropBuff_Operator()
	{
		bAllowMulitSameTypeBuff = true;
	}

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void WhenTick(float DeltaTime) override;
	FText GetStateName() const override;

	UPROPERTY(SaveGame, Replicated)
	TSubclassOf<UARPG_GameplayFloatPropertyOperatorBase> EffectProperty;

	UPROPERTY(SaveGame, Replicated)
	float TickAdditiveValue;
	UPROPERTY(SaveGame, Replicated)
	float TickMultipleValue = 1.f;
};

USTRUCT()
struct ARPG_API FPropSpecialBuffConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "持续时间")
	float Duration = 10.f;
	UPROPERTY(EditAnywhere, Category = "Buff", meta = (EditCondition = bAllowMulitSameTypeBuff, DisplayName = "覆盖方式"))
	EPropSpecialBuffOverlayType GeneralBuffOverlayType;

	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "起效的属性修改器")
	TArray<FPropBuffModifierConfig> EffectModifiers;
	UPROPERTY(EditAnywhere, Category = "Buff", DisplayName = "持续修改的属性")
	TArray<FPropBuffOperatorConfig> TickOperators;
};

UCLASS(abstract, meta = (DisplayName = "道具"))
class ARPG_API UARPG_PropCore_General : public UARPG_PropCoreBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "道具", meta = (DisplayName = "起效Buff"))
	TArray<TSubclassOf<UARPG_CharacterState_BuffBase>> Buffs;

	UPROPERTY(EditAnywhere, Category = "道具", meta = (DisplayName = "影响属性"))
	TArray<FGeneralPropOperatorConfig> EffectProps;

	UPROPERTY(EditAnywhere, Category = "道具", meta = (DisplayName = "起效的道具Buff"))
	FPropSpecialBuffConfig PropBuff;

	void UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput) override;
};
