// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include <Animation/AnimNotifies/AnimNotify.h>
#include <SubclassOf.h>
#include "ARPG_AnimNotify.generated.h"
/**
 * 
 */
UCLASS(meta = (DisplayName = "条件_动画跳转"))
class ARPG_API UARPG_PlayMontageByState : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_PlayMontageByState()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "跳转的蒙太奇"))
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "跳转片段名"))
	FName StartSectionName;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "客户端主导"))
	uint8 bClientMaster : 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "条件_动画跳转"))
class ARPG_API UARPG_PlayMontageCheckState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_PlayMontageCheckState() = default;

	UPROPERTY(EditAnywhere, Category = "动画")
	TSubclassOf<class UARPG_AnimPlayCondition> Condition;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "跳转的蒙太奇"))
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "跳转片段名"))
	FName StartSectionName;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "客户端主导"))
	uint8 bClientMaster : 1;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

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

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "判断松开状态"))
	uint8 bIsReleased : 1;

	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "EARPG_InputType"), Category = "动画", meta = (DisplayName = "输入类型"))
	int32 InputType;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "跳转的蒙太奇"))
	class UAnimMontage* Montage;
	
	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "跳转片段名"))
	FName StartSectionName;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "客户端主导"))
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

UCLASS(meta = (DisplayName = "动画_覆盖瞄准偏移"))
class ARPG_API UARPG_SetAimOffsetOverride : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "动画")
	class UAimOffsetBlendSpace* AimOffsetOverride;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(abstract, Blueprintable, const)
class ARPG_API UARPG_ApplyExecuteDamageFunctor : public UObject
{
	GENERATED_BODY()
public:
	virtual void ApplyExecuteDamage(class ACharacterBase* ExecuteTargetCharacter, class ACharacterBase* ExecuteFromCharacter) const
	{
		ReceiveApplyExecuteDamage(ExecuteTargetCharacter, ExecuteFromCharacter);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "伤害计算")
	void ReceiveApplyExecuteDamage(class ACharacterBase* ExecuteTargetCharacter, class ACharacterBase* ExecuteFromCharacter) const;
};

UCLASS(meta = (DisplayName = "数值_造成处决伤害"))
class ARPG_API UARPG_ApplyExecuteDamage : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_ApplyExecuteDamage()
	{
		bIsNativeBranchingPoint = true;
	}

	TSubclassOf<class UARPG_ApplyExecuteDamageFunctor> ApplyExecuteDamageFunctor;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};
