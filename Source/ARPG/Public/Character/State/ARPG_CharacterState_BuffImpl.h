// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterStateBase.h"
#include "ARPG_CharacterState_BuffImpl.generated.h"

class UARPG_GameplayFloatPropertyModifierBase;

/**
 * 
 */
UENUM()
enum class EGeneralBuffPropertyOperand : uint8
{
	None,
	Additive,
	Multiple
};

USTRUCT()
struct ARPG_API FGeneralBuffPropertyConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	TSubclassOf<UARPG_GameplayFloatPropertyModifierBase> ModifyProperty;
	UPROPERTY(EditAnywhere, Category = "Buff")
	EGeneralBuffPropertyOperand Operand;
	UPROPERTY(EditAnywhere, Category = "Buff")
	float Value;
};

UCLASS(meta = (DisplayName = "一般Buff"))
class ARPG_API UARPG_CS_Buff_General : public UARPG_CharacterState_BuffBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Buff")
	TArray<FGeneralBuffPropertyConfig> EffectPropertys;

	void WhenActived(bool IsFirstInit);
	void WhenTick(float DeltaTime);
	void WhenDeactived();
};
