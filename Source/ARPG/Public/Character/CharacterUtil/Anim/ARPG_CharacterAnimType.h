// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Kismet/BlueprintFunctionLibrary.h>
#include <SubclassOf.h>
#include <Animation/AnimMetaData.h>
#include "CharacterType.h"
#include "ARPG_CharacterConditionBase.h"
#include "ARPG_CharacterAnimType.generated.h"

/**
 * 
 */

UCLASS(const, Abstract, Blueprintable, BlueprintType, hidedropdown)
class UARPG_AnimPlayCondition : public UARPG_CharacterConditionBase
{
	GENERATED_BODY()
public:
	bool CalculateConditionValue_Implementation(class ACharacterBase* Character) const override;

	UFUNCTION(BlueprintNativeEvent, Category = "动画条件")
	bool CanPlayMontage(class ACharacterBase* Character) const;
	virtual bool CanPlayMontage_Implementation(class ACharacterBase* Character) const { return false; }
};

UCLASS()
class UMPC_WeaponAttackNormal : public UARPG_AnimPlayCondition
{
	GENERATED_BODY()
public:
	bool CanPlayMontage_Implementation(class ACharacterBase* Character) const override { return true; }
};

UCLASS(meta = (DisplayName = "能在下落时播放"))
class ARPG_API UAMD_CanPlayWhenFalling : public UAnimMetaData
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FARPG_MontageParameter
{
	GENERATED_BODY()
public:
	FARPG_MontageParameter()
		:Condition(UMPC_WeaponAttackNormal::StaticClass()),
		bClientMaster(true)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画", meta = (DisplayName = "播放条件"))
	TSubclassOf<class UARPG_AnimPlayCondition> Condition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画", meta = (DisplayName = "跳转的蒙太奇"))
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "动画", meta = (DisplayName = "跳转片段名"))
	FName StartSectionName;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "客户端主导"))
	uint8 bClientMaster : 1;
};

UCLASS()
class ARPG_API UARPG_AnimFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void CheckAndFixAnimData(UAnimMontage* Montage, TSubclassOf<class UAnimMetaData> AnimMetaType);
};

UCLASS(editinlinenew, Abstract, collapseCategories)
class ARPG_API UARPG_AttackAnimSetBase : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "攻击", BlueprintNativeEvent)
	void InvokePlay(class ACharacterBase* Character) const;
	virtual void InvokePlay_Implementation(class ACharacterBase* Character) const {}

	UFUNCTION(BlueprintCallable, Category = "攻击", BlueprintNativeEvent)
	void InvokePlayDodgeAnim(class ACharacterBase* Character, EDodgeDirection DodgeDirection) const;
	virtual void InvokePlayDodgeAnim_Implementation(class ACharacterBase* Character, EDodgeDirection DodgeDirection) const {}
};

UCLASS()
class ARPG_API UARPG_AttackAnimSetNormal : public UARPG_AttackAnimSetBase
{
	GENERATED_BODY()
public:
	virtual void InvokePlay_Implementation(class ACharacterBase* Character) const override;

	virtual void InvokePlayDodgeAnim_Implementation(class ACharacterBase* Character, EDodgeDirection DodgeDirection) const override;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "左手轻攻击"))
	FARPG_MontageParameter LeftLightAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "左手重攻击"))
	FARPG_MontageParameter LeftHeavyAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "右手重攻击"))
	FARPG_MontageParameter RightLightAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "右手重攻击"))
	FARPG_MontageParameter RightHeavyAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "左手冲刺攻击"))
	FARPG_MontageParameter LeftSprintAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "右手冲刺攻击"))
	FARPG_MontageParameter RightSprintAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "左手下落攻击"))
	FARPG_MontageParameter LeftFallingAttack;

	UPROPERTY(EditAnywhere, Category = "动画", meta = (DisplayName = "右手下落攻击"))
	FARPG_MontageParameter RightFallingAttack;

	UPROPERTY(EditDefaultsOnly, Category = "动画", meta = (DisplayName = "左手翻滚攻击"))
	FARPG_MontageParameter DodogeForwardLeftAttack;

	UPROPERTY(EditDefaultsOnly, Category = "动画", meta = (DisplayName = "右手翻滚攻击"))
	FARPG_MontageParameter DodogeForwardRightAttack;
};

UCLASS(editinlinenew, Abstract, collapseCategories)
class ARPG_API UARPG_DodgeAnimSetBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void InvokeDodge(class ACharacterBase* Character, EDodgeDirection Direction) const
	{
		ReceiveInvokeDodge(Character, Direction);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "闪避动画", meta = (DisplayName = "InvokeDodge"))
	void ReceiveInvokeDodge(class ACharacterBase* Character, EDodgeDirection Direction) const;

	virtual bool CanDodge(const class ACharacterBase* Character) const
	{
		return ReceiveCanDodge(Character);
	}
	UFUNCTION(BlueprintImplementableEvent, Category = "闪避动画", meta = (DisplayName = "CanDodge"))
	bool ReceiveCanDodge(const class ACharacterBase* Character) const;
};

UCLASS()
class ARPG_API UARPG_DodgeAnimSetNormal : public UARPG_DodgeAnimSetBase
{
	GENERATED_BODY()
public:
	virtual void InvokeDodge(class ACharacterBase* Character, EDodgeDirection Direction) const override;
	virtual bool CanDodge(const class ACharacterBase* Character) const override;

	UPROPERTY(EditDefaultsOnly, Category = "动画", meta = (DisplayName = "向前翻滚"))
	FARPG_MontageParameter DodgeForwardAnim;
	UPROPERTY(EditDefaultsOnly, Category = "动画", meta = (DisplayName = "向后翻滚"))
	FARPG_MontageParameter DodgeBackAnim;
	UPROPERTY(EditDefaultsOnly, Category = "动画", meta = (DisplayName = "向左翻滚"))
	FARPG_MontageParameter DodgeLeftAnim;
	UPROPERTY(EditDefaultsOnly, Category = "动画", meta = (DisplayName = "向右翻滚"))
	FARPG_MontageParameter DodgeRightAnim;
};
