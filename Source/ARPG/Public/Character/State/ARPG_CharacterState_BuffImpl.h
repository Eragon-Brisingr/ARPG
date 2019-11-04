// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterStateBase.h"
#include "ARPG_PropertyDef.h"
#include "ARPG_CharacterState_BuffImpl.generated.h"

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

UCLASS(meta = (DisplayName = "一般Buff"))
class ARPG_API UARPG_CS_Buff_General : public UARPG_CharacterState_BuffBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	TArray<FGeneralBuffModifierConfig> EffectModifiers;

	UPROPERTY(EditAnywhere, Category = "Buff")
	TArray<FGeneralBuffOperatorConfig> ActiveOperators;
	UPROPERTY(EditAnywhere, Category = "Buff")
	TArray<FGeneralBuffOperatorConfig> TickOperators;
	UPROPERTY(EditAnywhere, Category = "Buff")
	TArray<FGeneralBuffOperatorConfig> DeactiveOperators;

	void WhenActived(bool IsFirstInit);
	void WhenTick(float DeltaTime);
	void WhenDeactived();
};
