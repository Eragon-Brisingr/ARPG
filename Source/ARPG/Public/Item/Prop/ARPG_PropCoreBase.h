// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_PropertyDef.h"
#include "ARPG_PropCoreBase.generated.h"

class UARPG_CharacterState_BuffBase;
class UARPG_GameplayFloatPropertyOperatorBase;

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

UCLASS(abstract, meta = (DisplayName = "道具"))
class ARPG_API UARPG_PropCore_General : public UARPG_PropCoreBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "道具")
	TArray<TSubclassOf<UARPG_CharacterState_BuffBase>> Buffs;

	UPROPERTY(EditAnywhere, Category = "道具")
	TArray<FGeneralPropOperatorConfig> EffectProps;

	void UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput) override;
};
