// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include <Animation/AnimNotifies/AnimNotify.h>
#include <SubclassOf.h>
#include "HumanType.h"
#include "ARPG_AnimNotify.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "状态_动画跳转"))
class ARPG_API UARPG_PlayMontageByState : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_PlayMontageByState()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "动画")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "动画")
	FName StartSectionName;

	UPROPERTY(EditAnywhere, Category = "动画")
	uint8 bClientMaster : 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "输入_动画跳转"))
class ARPG_API UARPG_PlayMontageByInput : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_PlayMontageByInput()
		:bClientMaster(true)
	{}

	UPROPERTY(EditAnywhere, Category = "动画")
	uint8 bIsReleased : 1;

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "EARPG_InputType"), Category = "动画")
	int32 InputType;

	UPROPERTY(EditAnywhere, Category = "动画")
	class UAnimMontage* Montage;
	
	UPROPERTY(EditAnywhere, Category = "动画")
	FName StartSectionName;

	UPROPERTY(EditAnywhere, Category = "动画")
	uint8 bClientMaster : 1;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "状态_防御状态"))
class ARPG_API UARPG_DefenseState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "状态_防御反击状态"))
class ARPG_API UARPG_DefenseSwipeState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "状态_闪避状态"))
class ARPG_API UARPG_DodgeState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "属性_增加强韧度"))
class ARPG_API UARPG_AddToughnessValue : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "属性")
	float AddToughnessValue = 50.f;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

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

	UPROPERTY(EditAnywhere, Category = "武器位置")
	uint8 bPullOutWeapon : 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS(meta = (DisplayName = "人类_武器伤害检测"))
class ARPG_API UARPG_Human_WeaponTrace : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "攻击")
	uint8 bIsLeftWeapon : 1;

	UPROPERTY(EditAnywhere, Category = "攻击")
	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS(meta = (DisplayName = "人类_拔出箭"))
class ARPG_API UARPG_Human_PullOutArrow : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "攻击")
	uint8 bIsLeftWeapon : 1;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS(meta = (DisplayName = "人类_拉弓"))
class ARPG_API UARPG_Human_PullBow : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "攻击")
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

	UPROPERTY(EditAnywhere, Category = "攻击")
	uint8 bIsLeftWeapon : 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
