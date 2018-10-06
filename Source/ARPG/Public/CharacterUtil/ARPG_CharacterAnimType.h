// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include "ARPG_CharacterAnimType.generated.h"

/**
 * 
 */

UCLASS(const, Abstract, Blueprintable, BlueprintType)
class UARPG_MontagePlayCondition : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "条件")
	bool CanPlayMontage(class ACharacterBase* Character) const;
	virtual bool CanPlayMontage_Implementation(class ACharacterBase* Character) const { return false; }
};

UCLASS()
class UMPC_WeaponAttackNormal : public UARPG_MontagePlayCondition
{
	GENERATED_BODY()
public:
	virtual bool CanPlayMontage_Implementation(class ACharacterBase* Character) const { return true; }
};

USTRUCT(BlueprintType)
struct FARPG_MontageParameter
{
	GENERATED_BODY()
public:
	FARPG_MontageParameter()
		:Condition(UMPC_WeaponAttackNormal::StaticClass())
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画")
	TSubclassOf<class UARPG_MontagePlayCondition> Condition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画")
	class UAnimMontage* Montage;
};

USTRUCT(BlueprintType)
struct FHumanAttackAnimSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画")
	FARPG_MontageParameter LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画")
	FARPG_MontageParameter HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画")
	FARPG_MontageParameter SprintAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画")
	FARPG_MontageParameter DodogeForwardAttack;
};

UCLASS()
class ARPG_API UARPG_AnimFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
};
