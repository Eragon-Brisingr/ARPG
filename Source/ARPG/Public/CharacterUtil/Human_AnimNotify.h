// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CharacterType.h"
#include <SubclassOf.h>
#include "Human_AnimNotify.generated.h"

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

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

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

	UPROPERTY(EditAnywhere, Category = "武器位置", meta = (DisplayName = "为左手武器"))
	uint8 bPullOutWeapon : 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "人类_武器伤害检测"))
class ARPG_API UARPG_Human_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_Human_WeaponTrace()
		:bClearIgnoreList(true)
	{}

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "清空攻击到对象列表"))
	uint8 bClearIgnoreList : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "特殊受击动作"))
	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "削韧增加量"))
	float AddHitStunValue = 0.f;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "人类_下落攻击检测"))
class ARPG_API UARPG_Human_FallingAttackTrace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_Human_FallingAttackTrace()
		:bClearIgnoreList(true)
	{}

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "清空攻击到对象列表"))
	uint8 bClearIgnoreList : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "特殊受击动作"))
	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "削韧增加量"))
	float AddHitStunValue = 0.f;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS(meta = (DisplayName = "人类_拔出箭"))
class ARPG_API UARPG_Human_PullOutArrow : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS(meta = (DisplayName = "人类_拉弓"))
class ARPG_API UARPG_Human_PullBow : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
};

UCLASS(meta = (DisplayName = "人类_射出箭"))
class ARPG_API UARPG_Human_LaunchArrow : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_Human_LaunchArrow()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "为左手武器"))
	uint8 bIsLeftWeapon : 1;

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "拉满弓所需时间"))
	float FullBowTime = 2.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
