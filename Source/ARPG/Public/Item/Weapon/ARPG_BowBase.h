// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include "ARPG_BowBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "弓实体"))
class ARPG_API AARPG_BowBase : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	AARPG_BowBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY()
	class AARPG_ArrowBase* HoldingArrow;

	float HoldingTime;
	
	void SpawnArrowInHand();

	void LaunchArrow(float FullBowTime, const struct FApplyPointDamageParameter& ApplyPointDamageParameter);

public:
	//IARPG_AI_BattleInterface
	FTimerHandle BowRelease_TimerHandle;
	bool IsAllowedAttack_Implementation(class AActor* AttackTarget) const override;
	FVector GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const override;
	void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;
	void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond) override;
	//
	void AI_ReleaseArrow(FBP_OnAttackFinished OnAttackFinished);
};

UCLASS()
class ARPG_API AARPG_Bow_StaticMesh : public AARPG_BowBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Bow_StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void InitItemMesh() override { InitStaticMeshComponent(StaticMeshComponent); }
};

UCLASS()
class ARPG_API AARPG_Bow_SkeletalMesh : public AARPG_BowBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Bow_SkeletalMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitItemMesh() override { InitSkeletalMeshComponent(SkeletalMeshComponent); }
};
