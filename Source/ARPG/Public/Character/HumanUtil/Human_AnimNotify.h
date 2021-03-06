﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CharacterType.h"
#include <SubclassOf.h>
#include "ARPG_AnimNotify.h"
#include "Human_AnimNotify.generated.h"

class AHumanBase;
class AARPG_WeaponBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "输入_闪避攻击"))
class ARPG_API UARPG_DodgeAttack : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "动画")
	EDodgeDirection DodgeDirection;

	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "人类_武器位置"))
class ARPG_API UARPG_Human_TakeWeaponPos : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_Human_TakeWeaponPos()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "武器位置", meta = (DisplayName = "拔出武器"))
	uint8 bPullOutWeapon : 1;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(abstract)
class ARPG_API UARPG_Human_WeaponNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_Human_WeaponNotifyStateBase() = default;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;
	
	UPROPERTY(Transient)
	TMap<AHumanBase*, AARPG_WeaponBase*> ActiveWeaponMap;

	AARPG_WeaponBase* GetWeapon(AHumanBase* Human) const;

	void CacheWeapon(AHumanBase* Human, AARPG_WeaponBase* Weapon);
	AARPG_WeaponBase* FindCachedWeapon(const AHumanBase* Human) const;
	void ClearWeapon(AHumanBase* Human);
};

UCLASS(meta = (DisplayName = "人类_武器伤害检测"))
class ARPG_API UARPG_Human_WeaponTrace : public UARPG_Human_WeaponNotifyStateBase
{
	GENERATED_BODY()
public:
	UARPG_Human_WeaponTrace()
		:bClearIgnoreList(true)
	{}

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "清空攻击到对象列表"))
	uint8 bClearIgnoreList : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "伤害配置", ShowOnlyInnerProperties))
	FApplyPointDamageParameter PointDamageParameter;

	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "人类_下落攻击检测"))
class ARPG_API UARPG_Human_FallingAttackTrace : public UARPG_Human_WeaponNotifyStateBase
{
	GENERATED_BODY()
public:
	UARPG_Human_FallingAttackTrace()
		:bClearIgnoreList(true)
	{}

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "清空攻击到对象列表"))
	uint8 bClearIgnoreList : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "伤害配置", ShowOnlyInnerProperties))
	FApplyPointDamageParameter PointDamageParameter;

	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS(meta = (DisplayName = "人类_拔出箭"))
class ARPG_API UARPG_Human_PullOutArrow : public UARPG_Human_WeaponNotifyStateBase
{
	GENERATED_BODY()
public:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS(meta = (DisplayName = "人类_拉弓"))
class ARPG_API UARPG_Human_PullBow : public UARPG_Human_WeaponNotifyStateBase
{
	GENERATED_BODY()
public:
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS(meta = (DisplayName = "人类_射出箭"))
class ARPG_API UARPG_Human_LaunchArrow : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_Human_LaunchArrow()
	{
		bIsNativeBranchingPoint = true;
		PointDamageParameter.bCanDefense = false;
		PointDamageParameter.bCanDefenseSwipe = false;
	}
	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "拉满弓所需时间"))
	float FullBowTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "伤害配置", ShowOnlyInnerProperties))
	FApplyPointDamageParameter PointDamageParameter;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS(meta = (DisplayName = "人类左手武器"))
class ARPG_API USRDAF_LeftWeapon : public UARPG_SetReceiveDamageActionFunctorBase
{
	GENERATED_BODY()
public:
	void SetReceiveDamageAction(USkeletalMeshComponent* MeshComp, TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction) const override;
};

UCLASS(meta = (DisplayName = "人类右手武器"))
class ARPG_API USRDAF_RightWeapon : public UARPG_SetReceiveDamageActionFunctorBase
{
	GENERATED_BODY()
public:
	void SetReceiveDamageAction(USkeletalMeshComponent* MeshComp, TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction) const override;
};

UCLASS(meta = (DisplayName = "人类左手武器"))
class ARPG_API USAHSVF_LeftWeapon : public UARPG_SetAddHitStunValueFunctorBase
{
	GENERATED_BODY()
public:
	void SetAddHitStunValue(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const override;

	void Reset(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const override;
};

UCLASS(meta = (DisplayName = "人类右手武器"))
class ARPG_API USAHSVF_RightWeapon : public UARPG_SetAddHitStunValueFunctorBase
{
	GENERATED_BODY()
public:
	void SetAddHitStunValue(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const override;

	void Reset(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const override;
};

UCLASS(meta = (DisplayName = "人类左手武器"))
class ARPG_API USBBDF_LeftWeapon : public UARPG_SetBeakBackDistanceFunctorBase
{
	GENERATED_BODY()
public:
	void SetBeakBackDistance(USkeletalMeshComponent* MeshComp, float NormalBeakBackDistance, float DefenseBeakBackDistance) const override;
};

UCLASS(meta = (DisplayName = "人类右手武器"))
class ARPG_API USBBDF_RightWeapon : public UARPG_SetBeakBackDistanceFunctorBase
{
	GENERATED_BODY()
public:
	void SetBeakBackDistance(USkeletalMeshComponent* MeshComp, float NormalBeakBackDistance, float DefenseBeakBackDistance) const override;
};

UCLASS(meta = (DisplayName = "人类左手武器"))
class ARPG_API UDSF_LeftWeapon : public UARPG_DefenseStateFunctor
{
	GENERATED_BODY()
public:
	void DefenseBegin(USkeletalMeshComponent * MeshComp) const override;
	void DefenseEnd(USkeletalMeshComponent * MeshComp) const override;
};

UCLASS(meta = (DisplayName = "人类右手武器"))
class ARPG_API UDSF_RightWeapon : public UARPG_DefenseStateFunctor
{
	GENERATED_BODY()
public:
	void DefenseBegin(USkeletalMeshComponent * MeshComp) const override;
	void DefenseEnd(USkeletalMeshComponent * MeshComp) const override;
};
