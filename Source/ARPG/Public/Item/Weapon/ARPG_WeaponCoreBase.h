// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ItemCoreBase.h"
#include "SocketMoveTraceManager.h"
#include "ExecuteActionSet.h"
#include "ARPG_WeaponCoreBase.generated.h"

class UARPG_AttackAnimSetBase;

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponUseType : uint8
{
	SingleHand UMETA(DisplayName = "单持"),
	SingleHandForLeft UMETA(DisplayName = "单持左手"),
	SingleHandForRight UMETA(DisplayName = "单持右手"),
	BothHand UMETA(DisplayName = "双持"),
	BothHandForLeft UMETA(DisplayName = "双持左手"),
	BothHandForRight UMETA(DisplayName = "双持右手")
};

UCLASS(abstract, meta = (DisplayName = "武器"))
class ARPG_API UARPG_WeaponCoreBase : public UARPG_ItemCoreBase
{
	GENERATED_BODY()
public:
	UARPG_WeaponCoreBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FText GetItemTypeDesc() const override;
	void UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput) override;
	void WhenRemoveFromInventory(class AActor* ItemOwner, int32 RemoveNumber, int32 ExistNumber) override;
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
public:
	UPROPERTY(EditAnywhere, Category = "武器", meta = (DisplayName = "等级"))
	int32 Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "武器", meta = (DisplayName = "持武器模式"))
	EWeaponUseType WeaponUseType = EWeaponUseType::SingleHand;

	bool IsBothHandWeapon() const;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "武器", meta = (DisplayName = "基础削韧"))
	float BaseAddHitStunValue = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "武器", meta = (DisplayName = "基础物理攻击"))
	float BasePhysicsAttack = 100.f;

	float GetHitStunValue(float AnimAddHitStunValue) const { return BaseAddHitStunValue + AnimAddHitStunValue; }
	float GetPhysicsAttackValue() const { return BasePhysicsAttack; }

public:
	//只有双手武器可以起效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "武器", meta = (DisplayName = "移动动作覆盖", EditCondition = "WeaponUseType==WeaponUseType::BothHand||WeaponUseType == WeaponUseType::BothHandForLeft||WeaponUseType==WeaponUseType::BothHandForRight"))
	TSubclassOf<UAnimInstance> MoveAnimInstanceOverride;

	UPROPERTY(EditDefaultsOnly, Category = "武器", meta = (ShowOnlyInnerProperties = true))
	FSocketMoveTracerConfig SocketMoveTracerConfig;

	UPROPERTY(VisibleDefaultsOnly, Instanced, Category = "武器", meta = (ShowOnlyInnerProperties = true))
	UARPG_AttackAnimSetBase* AttackAnimSet;

	UPROPERTY(EditDefaultsOnly, Category = "武器", meta = (ShowOnlyInnerProperties = true))
	FExecuteActionSet ExecuteActionSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "武器", AdvancedDisplay, meta = (DisplayName = "左手持武器插槽名"))
	FName LeftWeaponInHandSocket = TEXT("weapon_hand_l");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "武器", AdvancedDisplay, meta = (DisplayName = "左手收回武器插槽名"))
	FName LeftWeaponInWeaponBackSocket = TEXT("weapon_back_r");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "武器", AdvancedDisplay, meta = (DisplayName = "右手持武器插槽名"))
	FName RightWeaponInHandSocket = TEXT("weapon_hand_r");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "武器", AdvancedDisplay, meta = (DisplayName = "右手收回武器插槽名"))
	FName RightWeaponInWeaponBackSocket = TEXT("weapon_back_l");
};
