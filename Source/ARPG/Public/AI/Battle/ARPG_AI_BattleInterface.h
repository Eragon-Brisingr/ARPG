// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARPG_AI_BattleInterface.generated.h"

DECLARE_DELEGATE_OneParam(FARPG_OnAttackFinished, bool /*Succeed*/);

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FBP_OnAttackFinished
{
	GENERATED_BODY()
public:
	FARPG_OnAttackFinished OnAttackFinished;

	void ExecuteIfBound(bool Succeed) const { OnAttackFinished.ExecuteIfBound(Succeed); }
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARPG_AI_BattleInterface : public UInterface
{
	GENERATED_BODY()
};
/**
 * 
 */
class ARPG_API IARPG_AI_BattleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗风格系统|意识")
	bool CanAttack(class AActor* AttackTarget) const;
	virtual bool CanAttack_Implementation(class AActor* AttackTarget) const { return false; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FVector GetAttackLocation(class AActor* AttackTarget) const;
	virtual FVector GetAttackLocation_Implementation(class AActor* AttackTarget) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FRotator GetAttackRotation(class AActor* AttackTarget) const;
	virtual FRotator GetAttackRotation_Implementation(class AActor* AttackTarget) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void InvokeAttack(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished);
	virtual void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void AbortAttack(class AActor* AttackTarget);
	virtual void AbortAttack_Implementation(class AActor* AttackTarget) {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void AttackingTick(class AActor* AttackTarget, float DeltaSecond);
	virtual void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond){}
};

UCLASS()
class ARPG_API UARPG_BattleInterfaceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	void ExecuteAttackFinished(const FBP_OnAttackFinished& OnAttackFinished, bool Succeed = true) { OnAttackFinished.ExecuteIfBound(Succeed); }
};
