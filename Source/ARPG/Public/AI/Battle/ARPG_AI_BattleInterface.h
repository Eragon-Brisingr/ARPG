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
	FBP_OnAttackFinished() = default;
	FBP_OnAttackFinished(const FARPG_OnAttackFinished& OnAttackFinished)
		:OnAttackFinished(OnAttackFinished)
	{}

	FARPG_OnAttackFinished OnAttackFinished;

	void ExecuteIfBound(bool Succeed) const { OnAttackFinished.ExecuteIfBound(Succeed); }
};

DECLARE_DELEGATE(FARPG_OnAttackAborted);

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FBP_OnAttackAborted
{
	GENERATED_BODY()
public:
	FBP_OnAttackAborted() = default;
	FBP_OnAttackAborted(const FARPG_OnAttackAborted& OnAttackAborted)
		:OnAttackAborted(OnAttackAborted)
	{}

	FARPG_OnAttackAborted OnAttackAborted;

	void ExecuteIfBound() const { OnAttackAborted.ExecuteIfBound(); }
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	bool CanAttack(class AActor* AttackTarget) const;
	virtual bool CanAttack_Implementation(class AActor* AttackTarget) const;
	static bool CanAttack(const UObject* Obj, class AActor* AttackTarget) { return IARPG_AI_BattleInterface::Execute_CanAttack(Obj, AttackTarget); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FVector GetAttackMoveLocation(class AActor* AttackTarget) const;
	virtual FVector GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const;
	static FVector GetAttackMoveLocation(const UObject* Obj, class AActor* AttackTarget) { return IARPG_AI_BattleInterface::Execute_GetAttackMoveLocation(Obj, AttackTarget); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FRotator GetAttackFaceRotation(class AActor* AttackTarget) const;
	virtual FRotator GetAttackFaceRotation_Implementation(class AActor* AttackTarget) const;
	static FRotator GetAttackFaceRotation(const UObject* Obj, class AActor* AttackTarget) { return IARPG_AI_BattleInterface::Execute_GetAttackFaceRotation(Obj, AttackTarget); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	bool IsAllowedAttack(class AActor* AttackTarget) const;
	virtual bool IsAllowedAttack_Implementation(class AActor* AttackTarget) const;
	static bool IsAllowedAttack(UObject* Obj, class AActor* AttackTarget) { return IARPG_AI_BattleInterface::Execute_IsAllowedAttack(Obj, AttackTarget); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void InvokeAttack(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished);
	virtual void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished);
	static void InvokeAttack(UObject* Obj, class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) { IARPG_AI_BattleInterface::Execute_InvokeAttack(Obj, AttackTarget, OnAttackFinished); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void AbortAttack(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted);
	virtual void AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted);
	static void AbortAttack(UObject* Obj, class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) { IARPG_AI_BattleInterface::Execute_AbortAttack(Obj, AttackTarget, OnAttackAborted); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void AttackingTick(class AActor* AttackTarget, float DeltaSecond);
	virtual void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond);
	static void AttackingTick(UObject* Obj, class AActor* AttackTarget, float DeltaSecond) { IARPG_AI_BattleInterface::Execute_AttackingTick(Obj, AttackTarget, DeltaSecond); }
};

UCLASS()
class ARPG_API UARPG_BattleInterfaceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ExecuteAttackFinished(const FBP_OnAttackFinished& OnAttackFinished, bool Succeed = true) { OnAttackFinished.ExecuteIfBound(Succeed); }

	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ExecuteAttackAborted(const FBP_OnAttackAborted& OnAttackFinished) { OnAttackFinished.ExecuteIfBound(); }
};
