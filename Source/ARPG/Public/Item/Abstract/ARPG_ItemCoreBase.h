// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ItemCoreBase.h"
#include "ItemTypeUtils.h"
#include "ARPG_ItemCoreBase.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "物品"))
class ARPG_API UARPG_ItemCoreBase : public UXD_ItemCoreBase
{
	GENERATED_BODY()
public:
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
public:
	UARPG_ItemCoreBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, Category = "物品", meta = (DisplayName = "重量"))
	float Weight = 0.1f;

	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, Category = "物品", meta = (DisplayName = "价格"))
	float Price = 1.f;

	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, Category = "物品", meta = (DisplayName = "描述"))
	FText Describe;

public:
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	virtual float GetWeight() const { return ReceiveGetWeight(); }
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "物品|基础", meta = (DisplayName = "GetWeight"))
	float ReceiveGetWeight() const;
	float ReceiveGetWeight_Implementation() const { return Weight; }

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	virtual float GetPrice() const { return ReceiveGetPrice(); }
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "物品|基础", meta = (DisplayName = "GetPrice"))
	float ReceiveGetPrice() const;
	float ReceiveGetPrice_Implementation() const { return Price; }

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	virtual FText GetItemTypeDesc() const { return ReceiveGetItemTypeDesc(); }
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "物品|基础")
	FText ReceiveGetItemTypeDesc() const;
	FText ReceiveGetItemTypeDesc_Implementation() const;
	
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	FText GetDescribe() const { return Describe; }
	
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetTradePrice(class UXD_InventoryComponentBase* Invoker, class UXD_InventoryComponentBase* Trader, ETradePart InvokerTradePart) const;
public:
	virtual void UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput);
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "UseItem"))
	void ReceiveUseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput);

	virtual void WhenUse(ACharacterBase* ItemOwner) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "WhenUse"))
	void ReceiveWhenUse(ACharacterBase* ItemOwner);
	virtual void WhenNotUse(ACharacterBase* ItemOwner) {}
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "WhenNotUse"))
	void ReceiveWhenNotUse(ACharacterBase* ItemOwner);

	//使用物品动画
public:
	UPROPERTY(EditDefaultsOnly, Category = "物品", meta = (DisplayName = "使用时动画"))
	UAnimMontage* UseItemMontage;
	UPROPERTY(EditDefaultsOnly, Category = "物品", meta = (DisplayName = "使用时位置插槽"))
	FName UseItemAttachSocketName;
};
