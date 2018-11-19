// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include <Animation/AnimNotifies/AnimNotify.h>
#include <SubclassOf.h>
#include <Engine/EngineTypes.h>
#include "SocketMoveTraceManager.h"
#include "CharacterDamageType.h"
#include "ARPG_BattleType.h"
#include "ARPG_AnimNotify.generated.h"
/**
 * 
 */
UENUM()
enum class EARPG_AnimDebugHintType : uint8
{
	None UMETA(DisplayName = "无"),
	OnlyPreview UMETA(DisplayName = "仅预览"),
	All UMETA(DisplayName = "所有情况")
};

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

UCLASS(meta = (DisplayName = "输入_闪避"))
class ARPG_API UARPG_DodgeAnim : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(abstract, Blueprintable, const)
class ARPG_API UARPG_DefenseStateFunctor : public UObject
{
	GENERATED_BODY()
public:
	virtual void DefenseBegin(USkeletalMeshComponent * MeshComp) const
	{
		ReceiveDefenseBegin(MeshComp);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "防御")
	void ReceiveDefenseBegin(USkeletalMeshComponent * MeshComp) const;

	virtual void DefenseEnd(USkeletalMeshComponent * MeshComp) const
	{
		ReceiveDefenseEnd(MeshComp);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "防御")
	void ReceiveDefenseEnd(USkeletalMeshComponent * MeshComp) const;
};

UCLASS(meta = (DisplayName = "状态_防御状态"))
class ARPG_API UARPG_DefenseState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "防御", meta = (DisplayName = "防御起效部位"))
	TSubclassOf<class UARPG_DefenseStateFunctor> DefenseStateFunctor;
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

UCLASS(meta = (DisplayName = "检测_启用攻击检测"))
class ARPG_API UARPG_EnableAttackTracer : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_EnableAttackTracer();

	UPROPERTY(EditAnywhere, Category = "攻击检测", meta = (DisplayName = "检测配置", ShowOnlyInnerProperties))
	FSocketMoveTracerConfig SocketMoveTracerConfig;

	UPROPERTY(EditAnywhere, Category = "攻击检测", meta = (DisplayName = "伤害配置", ShowOnlyInnerProperties))
	FApplyPointDamageParameter PointDamageParameter;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	class USocketMoveTracer* PreviewSocketMoveTracer;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
#endif
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UENUM(BlueprintType)
enum class ESettableMovement : uint8
{
	Fly UMETA(DisplayName = "飞行")
};

UCLASS(meta = (DisplayName = "状态_设置移动模式"))
class ARPG_API UARPG_SetMovementMode : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "移动模式")
	ESettableMovement MovementMode = ESettableMovement::Fly;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	mutable UEnum* MovementModeEnum;
#endif
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

	UPROPERTY(EditAnywhere, Category = "处决", meta = (DisplayName = "处决伤害计算"))
	TSubclassOf<class UARPG_ApplyExecuteDamageFunctor> ApplyExecuteDamageFunctor;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(Blueprintable, abstract, const)
class ARPG_API UARPG_SetReceiveDamageActionFunctorBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void SetReceiveDamageAction(USkeletalMeshComponent* MeshComp, TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction) const
	{
		ReceiveSetReceiveDamageAction(MeshComp, ReceiveDamageAction);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "函数子", meta = (DisplayName = "SetReceiveDamageAction"))
	void ReceiveSetReceiveDamageAction(USkeletalMeshComponent* MeshComp, TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction) const;
};

UCLASS(meta = (DisplayName = "动画_设置特殊受击动画"))
class ARPG_API UARPG_SetReceiveDamageAction : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_SetReceiveDamageAction()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "作用对象"))
	TSubclassOf<class UARPG_SetReceiveDamageActionFunctorBase> SetReceiveDamageActionFunctor;
	
	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "特殊受击动画"))
	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(Blueprintable, abstract, const)
class ARPG_API UARPG_SetAddHitStunValueFunctorBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void SetAddHitStunValue(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const
	{
		ReceiveSetAddHitStunValue(MeshComp, AddHitStunValue);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "函数子", meta = (DisplayName = "SetAddHitStunValue"))
	void ReceiveSetAddHitStunValue(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const;

	virtual void Reset(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const
	{
		ReceiveReset(MeshComp, AddHitStunValue);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "函数子", meta = (DisplayName = "Reset"))
	void ReceiveReset(USkeletalMeshComponent* MeshComp, float AddHitStunValue) const;
};

UCLASS(meta = (DisplayName = "动画_设置动画增加削韧量"))
class ARPG_API UARPG_SetAddHitStunValue : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_SetAddHitStunValue()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "削韧增加量"))
	float AddHitStunValue = 0.f;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "作用对象"))
	TSubclassOf<class UARPG_SetAddHitStunValueFunctorBase> SetAddHitStunValueFunctor;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(abstract, Blueprintable, const)
class ARPG_API UARPG_SetBeakBackDistanceFunctorBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void SetBeakBackDistance(USkeletalMeshComponent* MeshComp, float NormalBeakBackDistance, float DefenseBeakBackDistance) const
	{
		ReceiveBeakBackDistance(MeshComp, NormalBeakBackDistance, DefenseBeakBackDistance);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "函数子", meta = (DisplayName = "BeakBackDistance"))
	void ReceiveBeakBackDistance(USkeletalMeshComponent* MeshComp, float NormalBeakBackDistance, float DefenseBeakBackDistance) const;
};

UCLASS(meta = (DisplayName = "动画_设置击退距离"))
class ARPG_API UARPG_SetBeakBackDistance : public UAnimNotify
{
	GENERATED_BODY()
public:
	UARPG_SetBeakBackDistance()
	{
		bIsNativeBranchingPoint = true;
	}

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "击退距离"))
	float NormalBeakBackDistance = 20.f;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "防御击退距离"))
	float DefenseBeakBackDistance = 20.f;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "作用对象"))
	TSubclassOf<class UARPG_SetBeakBackDistanceFunctorBase> SetBeakBackDistanceFunctor;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	virtual FString GetNotifyName_Implementation() const override;
};

UCLASS(meta = (DisplayName = "AI_设置攻击信息"))
class ARPG_API UARPG_SetAttackInfo : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UARPG_SetAttackInfo();

	UPROPERTY(EditAnywhere, Category = "攻击", meta = (DisplayName = "攻击信息"))
	TArray<FARPG_AttackInfo> AttackInfos;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	virtual FString GetNotifyName_Implementation() const override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "攻击")
	EARPG_AnimDebugHintType DebugHintType = EARPG_AnimDebugHintType::OnlyPreview;

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
#endif
};
