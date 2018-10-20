// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Abstract/ARPG_ItemBase.h"
#include "ARPG_CharacterAnimType.h"
#include "ExecuteActionSet.h"
#include "ARPG_WeaponBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponUseType : uint8
{
	SingleHand UMETA(DisplayName = "单持"),
	SingleHandForLeft UMETA(DisplayName = "单持左手"),
	SingleHandForRight UMETA(DisplayName = "单持右手"),
	BothHandForLeft UMETA(DisplayName = "双持左手"),
	BothHandForRight UMETA(DisplayName = "双持右手")
};


UCLASS(Abstract)
class ARPG_API AARPG_WeaponBase : public AARPG_ItemBase
{
	GENERATED_BODY()
	
public:
	AARPG_WeaponBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const override;

	virtual FText GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const override;

	virtual void WhenUse(class ACharacterBase* ItemOwner) override;

	virtual void WhenNotUse(class ACharacterBase* ItemOwner) override;

	virtual void WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const override;

	virtual void PostInitializeComponents();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "物品|武器", AdvancedDisplay, meta = (DisplayName = "持武器模式"))
	EWeaponUseType WeaponUseType = EWeaponUseType::SingleHand;

	UPROPERTY(VisibleAnywhere, Category = "物品", Instanced)
	class USocketMoveTracer* SocketMoveTracer;

	//攻击
public:
	void SetEnableNearAttackTrace(bool Enable);

	void WhenAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult);

	TSubclassOf<class UReceiveDamageActionBase> ReceiveDamageAction;

	UPROPERTY(EditDefaultsOnly, Category = "物品|武器", meta = (ShowOnlyInnerProperties))
	FExecuteActionSet ExecuteActionSet;

	UFUNCTION(BlueprintCallable, Category = "武器|行为")
	bool TraceForExecuteOther();

	void SetEnableFallingAttackTrace(bool Enable);

	void WhenFallingAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult);

	//持武器方式
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "物品|武器", AdvancedDisplay, meta = (DisplayName = "左手持武器插槽名"))
	FName LeftWeaponInHandSocket = TEXT("weapon_hand_l");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "物品|武器", AdvancedDisplay, meta = (DisplayName = "左手收回武器插槽名"))
	FName LeftWeaponInWeaponBackSocket = TEXT("weapon_back_r");
	virtual void WhenInHand();
	//拔出武器发生的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|武器", meta = (DisplayName = "WhenInHand"))
	void ReceiveWhenInHand();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "物品|武器", AdvancedDisplay, meta = (DisplayName = "右手持武器插槽名"))
	FName RightWeaponInHandSocket = TEXT("weapon_hand_r");
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "物品|武器", AdvancedDisplay, meta = (DisplayName = "右手收回武器插槽名"))
	FName RightWeaponInWeaponBackSocket = TEXT("weapon_back_l");
	virtual void WhenInWeaponBack();
	//收回武器发生的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|武器", meta = (DisplayName = "WhenInWeaponBack"))
	void ReceiveWhenInWeaponBack();

	void AttachWeaponTo(class USceneComponent* InParent, FName InSocketName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "动画", Instanced)
	class UARPG_AttackAnimSetBase* AttackAnimSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "动画")
	FARPG_MontageParameter DodogeForwardLeftAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "动画")
	FARPG_MontageParameter DodogeForwardRightAttack;
};
