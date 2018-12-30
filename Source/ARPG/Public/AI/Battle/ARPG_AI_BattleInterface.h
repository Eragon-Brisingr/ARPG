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

DECLARE_DELEGATE_OneParam(FARPG_OnDefenseFinished, bool /*Succeed*/);

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FBP_OnDefenseFinished
{
	GENERATED_BODY()
public:
	FBP_OnDefenseFinished() = default;
	FBP_OnDefenseFinished(const FARPG_OnDefenseFinished& OnDefenseFinished)
		:OnDefenseFinished(OnDefenseFinished)
	{}

	FARPG_OnDefenseFinished OnDefenseFinished;

	void ExecuteIfBound(bool Succeed) const { OnDefenseFinished.ExecuteIfBound(Succeed); }
};

DECLARE_DELEGATE(FARPG_OnDefenseAborted);

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FBP_OnDefenseAborted
{
	GENERATED_BODY()
public:
	FBP_OnDefenseAborted() = default;
	FBP_OnDefenseAborted(const FARPG_OnDefenseAborted& OnDefenseAborted)
		:OnDefenseAborted(OnDefenseAborted)
	{}

	FARPG_OnDefenseAborted OnDefenseAborted;

	void ExecuteIfBound() const { OnDefenseAborted.ExecuteIfBound(); }
};

DECLARE_DELEGATE_OneParam(FARPG_OnDefenseSwipeFinished, bool /*Succeed*/);

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FBP_OnDefenseSwipeFinished
{
	GENERATED_BODY()
public:
	FBP_OnDefenseSwipeFinished() = default;
	FBP_OnDefenseSwipeFinished(const FARPG_OnDefenseSwipeFinished& OnDefenseSwipeFinished)
		:OnDefenseSwipeFinished(OnDefenseSwipeFinished)
	{}

	FARPG_OnDefenseSwipeFinished OnDefenseSwipeFinished;

	void ExecuteIfBound(bool Succeed) const { OnDefenseSwipeFinished.ExecuteIfBound(Succeed); }
};

DECLARE_DELEGATE(FARPG_OnDefenseSwipeAborted);

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FBP_OnDefenseSwipeAborted
{
	GENERATED_BODY()
public:
	FBP_OnDefenseSwipeAborted() = default;
	FBP_OnDefenseSwipeAborted(const FARPG_OnDefenseSwipeAborted& OnDefenseSwipeAborted)
		:OnDefenseSwipeAborted(OnDefenseSwipeAborted)
	{}

	FARPG_OnDefenseSwipeAborted OnDefenseSwipeAborted;

	void ExecuteIfBound() const { OnDefenseSwipeAborted.ExecuteIfBound(); }
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

	//Attack
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


	//Defense
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	bool CanDefense(class AActor* DefenseTarget) const;
	virtual bool CanDefense_Implementation(class AActor* DefenseTarget) const;
	static bool CanDefense(const UObject* Obj, class AActor* DefenseTarget) { return IARPG_AI_BattleInterface::Execute_CanDefense(Obj, DefenseTarget); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FVector GetDefenseMoveLocation(class AActor* DefenseTarget) const;
	virtual FVector GetDefenseMoveLocation_Implementation(class AActor* DefenseTarget) const;
	static FVector GetDefenseMoveLocation(const UObject* Obj, class AActor* DefenseTarget) { return IARPG_AI_BattleInterface::Execute_GetDefenseMoveLocation(Obj, DefenseTarget); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FRotator GetDefenseFaceRotation(class AActor* DefenseTarget) const;
	virtual FRotator GetDefenseFaceRotation_Implementation(class AActor* DefenseTarget) const;
	static FRotator GetDefenseFaceRotation(const UObject* Obj, class AActor* DefenseTarget) { return IARPG_AI_BattleInterface::Execute_GetDefenseFaceRotation(Obj, DefenseTarget); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	bool IsAllowedDefense(class AActor* DefenseTarget) const;
	virtual bool IsAllowedDefense_Implementation(class AActor* DefenseTarget) const;
	static bool IsAllowedDefense(UObject* Obj, class AActor* DefenseTarget) { return IARPG_AI_BattleInterface::Execute_IsAllowedDefense(Obj, DefenseTarget); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void InvokeDefense(class AActor* DefenseTarget, const FBP_OnDefenseFinished& OnDefenseFinished);
	virtual void InvokeDefense_Implementation(class AActor* DefenseTarget, const FBP_OnDefenseFinished& OnDefenseFinished);
	static void InvokeDefense(UObject* Obj, class AActor* DefenseTarget, const FBP_OnDefenseFinished& OnDefenseFinished) { IARPG_AI_BattleInterface::Execute_InvokeDefense(Obj, DefenseTarget, OnDefenseFinished); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void AbortDefense(class AActor* DefenseTarget, const FBP_OnDefenseAborted& OnDefenseAborted);
	virtual void AbortDefense_Implementation(class AActor* DefenseTarget, const FBP_OnDefenseAborted& OnDefenseAborted);
	static void AbortDefense(UObject* Obj, class AActor* DefenseTarget, const FBP_OnDefenseAborted& OnDefenseAborted) { IARPG_AI_BattleInterface::Execute_AbortDefense(Obj, DefenseTarget, OnDefenseAborted); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void DefenseingTick(class AActor* DefenseTarget, float DeltaSecond);
	virtual void DefenseingTick_Implementation(class AActor* DefenseTarget, float DeltaSecond);
	static void DefenseingTick(UObject* Obj, class AActor* DefenseTarget, float DeltaSecond) { IARPG_AI_BattleInterface::Execute_DefenseingTick(Obj, DefenseTarget, DeltaSecond); }

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	bool CanDefenseSwipe(class AActor* DefenseSwipeTarget) const;
	virtual bool CanDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget) const;
	static bool CanDefenseSwipe(const UObject* Obj, class AActor* DefenseSwipeTarget) { return IARPG_AI_BattleInterface::Execute_CanDefenseSwipe(Obj, DefenseSwipeTarget); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FVector GetDefenseSwipeMoveLocation(class AActor* DefenseSwipeTarget) const;
	virtual FVector GetDefenseSwipeMoveLocation_Implementation(class AActor* DefenseSwipeTarget) const;
	static FVector GetDefenseSwipeMoveLocation(const UObject* Obj, class AActor* DefenseSwipeTarget) { return IARPG_AI_BattleInterface::Execute_GetDefenseSwipeMoveLocation(Obj, DefenseSwipeTarget); }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "战斗")
	FRotator GetDefenseSwipeFaceRotation(class AActor* DefenseSwipeTarget) const;
	virtual FRotator GetDefenseSwipeFaceRotation_Implementation(class AActor* DefenseSwipeTarget) const;
	static FRotator GetDefenseSwipeFaceRotation(const UObject* Obj, class AActor* DefenseSwipeTarget) { return IARPG_AI_BattleInterface::Execute_GetDefenseSwipeFaceRotation(Obj, DefenseSwipeTarget); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	bool IsAllowedDefenseSwipe(class AActor* DefenseSwipeTarget) const;
	virtual bool IsAllowedDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget) const;
	static bool IsAllowedDefenseSwipe(UObject* Obj, class AActor* DefenseSwipeTarget) { return IARPG_AI_BattleInterface::Execute_IsAllowedDefenseSwipe(Obj, DefenseSwipeTarget); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void InvokeDefenseSwipe(class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeFinished& OnDefenseSwipeFinished);
	virtual void InvokeDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeFinished& OnDefenseSwipeFinished);
	static void InvokeDefenseSwipe(UObject* Obj, class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeFinished& OnDefenseSwipeFinished) { IARPG_AI_BattleInterface::Execute_InvokeDefenseSwipe(Obj, DefenseSwipeTarget, OnDefenseSwipeFinished); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void AbortDefenseSwipe(class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeAborted& OnDefenseSwipeAborted);
	virtual void AbortDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeAborted& OnDefenseSwipeAborted);
	static void AbortDefenseSwipe(UObject* Obj, class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeAborted& OnDefenseSwipeAborted) { IARPG_AI_BattleInterface::Execute_AbortDefenseSwipe(Obj, DefenseSwipeTarget, OnDefenseSwipeAborted); }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "战斗")
	void DefenseSwipeingTick(class AActor* DefenseSwipeTarget, float DeltaSecond);
	virtual void DefenseSwipeingTick_Implementation(class AActor* DefenseSwipeTarget, float DeltaSecond);
	static void DefenseSwipeingTick(UObject* Obj, class AActor* DefenseSwipeTarget, float DeltaSecond) { IARPG_AI_BattleInterface::Execute_DefenseSwipeingTick(Obj, DefenseSwipeTarget, DeltaSecond); }
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

	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ExecuteDefenseFinished(const FBP_OnDefenseFinished& OnDefenseFinished, bool Succeed = true) { OnDefenseFinished.ExecuteIfBound(Succeed); }

	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ExecuteDefenseAborted(const FBP_OnDefenseAborted& OnDefenseFinished) { OnDefenseFinished.ExecuteIfBound(); }

	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ExecuteDefenseSwipeFinished(const FBP_OnDefenseSwipeFinished& OnDefenseSwipeFinished, bool Succeed = true) { OnDefenseSwipeFinished.ExecuteIfBound(Succeed); }

	UFUNCTION(BlueprintCallable, Category = "游戏|AI")
	static void ExecuteDefenseSwipeAborted(const FBP_OnDefenseSwipeAborted& OnDefenseSwipeFinished) { OnDefenseSwipeFinished.ExecuteIfBound(); }
};
