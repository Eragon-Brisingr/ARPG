// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include <SubclassOf.h>
#include "ARPG_CharacterAnimType.generated.h"

/**
 * 
 */

UCLASS(const, Abstract, Blueprintable, BlueprintType)
class UARPG_AnimPlayCondition : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, Category = "条件")
	bool CanPlayMontage(class ACharacterBase* Character) const;
	virtual bool CanPlayMontage_Implementation(class ACharacterBase* Character) const { return false; }
};

UCLASS()
class UMPC_WeaponAttackNormal : public UARPG_AnimPlayCondition
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
	TSubclassOf<class UARPG_AnimPlayCondition> Condition;

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

UCLASS(editinlinenew, Abstract, collapseCategories)
class ARPG_API UARPG_AttackAnimSetBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "攻击", BlueprintNativeEvent)
	void InvokePlay(class ACharacterBase* Character) const;
	virtual void InvokePlay_Implementation(class ACharacterBase* Character) const {}
};

UCLASS()
class ARPG_API UARPG_AttackAnimSetNormal : public UARPG_AttackAnimSetBase
{
	GENERATED_BODY()
public:
	virtual void InvokePlay_Implementation(class ACharacterBase* Character) const override;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter LeftLightAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter LeftHeavyAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter RightLightAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter RightHeavyAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter LeftSprintAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter RightSprintAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter LeftFallingAttack;

	UPROPERTY(EditAnywhere, Category = "动画")
	FARPG_MontageParameter RightFallingAttack;
};
