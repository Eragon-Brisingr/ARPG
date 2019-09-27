// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ItemBase.h"
#include "ARPG_CharacterAnimType.h"
#include "ExecuteActionSet.h"
#include "SocketMoveTraceManager.h"
#include "CharacterDamageType.h"
#include "ARPG_AI_BattleInterface.h"
#include "ARPG_WeaponBase.generated.h"

class UARPG_WeaponCoreBase;

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "武器"))
class ARPG_API AARPG_WeaponBase : public AARPG_ItemBase, public IARPG_AI_BattleInterface
{
	GENERATED_BODY()
	
public:
	AARPG_WeaponBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void WhenUse(class ACharacterBase* ItemOwner) override;

	void WhenNotUse(class ACharacterBase* ItemOwner) override;

	//IARPG_AI_BattleInterface
public:
	//IARPG_AI_BattleInterface
	bool CanAttack_Implementation(class AActor* AttackTarget) const override { return true; }
	FVector GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const override;
	FRotator GetAttackFaceRotation_Implementation(class AActor* AttackTarget) const override;
	bool IsAllowedAttack_Implementation(class AActor* AttackTarget) const override;
	void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;
	void AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) override;
	//
	FTimerHandle FinishAttack_TimeHandle;
	void FinishAttack(FBP_OnAttackFinished OnAttackFinished);

	//攻击
public:
	UPROPERTY()
	class USocketMoveTracer* SocketMoveTracer;

	void EnableNearAttackTrace(const FApplyPointDamageParameter& ApplyPointDamageParameter, bool ClearIgnoreList = true);
	void DisableNearAttackTrace();
	void WhenAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult);

	UFUNCTION(BlueprintCallable, Category = "武器|行为")
	bool TraceForExecuteOther();

	void EnableFallingAttackTrace(const FApplyPointDamageParameter& ApplyPointDamageParameter, bool ClearIgnoreList = true);
	void DisableFallingAttackTrace();

	void WhenFallingAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult);

	FApplyPointDamageParameter PointDamageParameter;

	//持武器方式
public:
	virtual void WhenInHand();
	//拔出武器发生的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "武器", meta = (DisplayName = "WhenInHand"))
	void ReceiveWhenInHand();

	virtual void WhenInWeaponBack();
	//收回武器发生的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "武器", meta = (DisplayName = "WhenInWeaponBack"))
	void ReceiveWhenInWeaponBack();

	void AttachWeaponTo(class USceneComponent* InParent, FName InSocketName);

	bool IsBothHandWeapon() const;

public:
	UFUNCTION(BlueprintCallable, Category = "物品|武器")
	UARPG_AttackAnimSetBase* GetAttackAnimSet() const;

public:
	const UARPG_WeaponCoreBase* GetItemCoreConst() const;
	UARPG_WeaponCoreBase* GetItemCore() const;
};

UCLASS()
class ARPG_API AARPG_Weapon_StaticMesh : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Weapon_StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void InitItemMesh() override { InitStaticMeshComponent(StaticMeshComponent); }
};

UCLASS()
class ARPG_API AARPG_Weapon_SkeletalMesh : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Weapon_SkeletalMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitItemMesh() override { InitSkeletalMeshComponent(SkeletalMeshComponent); }
};
