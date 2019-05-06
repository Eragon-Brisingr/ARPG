// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ItemBase.h"
#include "ItemTypeUtils.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_ItemBase.generated.h"

class ACharacterBase;
class UXD_ItemCoreBase;
class UARPG_ItemCoreBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisplayName = "物品"))
class ARPG_API AARPG_ItemBase : public AXD_ItemBase, public IARPG_InteractInterface
{
	GENERATED_BODY()
public:
	//Begin IARPG_InteractInterface
	virtual void WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker) override;
	virtual bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const override;
	//End IARPG_InteractInterface
public:
	AARPG_ItemBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, Category = "物品", meta = (DisplayName = "重量"))
	float Weight;

	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, Category = "物品", meta = (DisplayName = "价格"))
	float Price;

	UPROPERTY(SaveGame, EditDefaultsOnly, BlueprintReadOnly, Category = "物品", meta = (DisplayName = "描述"))
	FText Describe;

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetWeight() const { return GetWeightImpl(InnerItemCore); }

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetPrice() const { return GetPriceImpl(InnerItemCore); }

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	FText GetItemTypeDesc() const { return GetItemTypeDescImpl(InnerItemCore); }
	
	UFUNCTION(BlueprintNativeEvent, Category = "物品|基础")
	float GetWeightImpl(const UXD_ItemCoreBase* ItemCore) const;
	virtual float GetWeightImpl_Implementation(const UXD_ItemCoreBase* ItemCore) const { return Weight; }

	UFUNCTION(BlueprintNativeEvent, Category = "物品|基础")
	float GetPriceImpl(const UXD_ItemCoreBase* ItemCore) const;
	virtual float GetPriceImpl_Implementation(const UXD_ItemCoreBase* ItemCore) const { return Price; }

	UFUNCTION(BlueprintNativeEvent, Category = "物品|基础")
	FText GetItemTypeDescImpl(const UXD_ItemCoreBase* ItemCore) const;
	virtual FText GetItemTypeDescImpl_Implementation(const UXD_ItemCoreBase* ItemCore) const;

	UFUNCTION(BlueprintCallable, Category = "物品|基础")
	ACharacterBase* GetItemOwner() const;

	UFUNCTION(BlueprintCallable, Category = "物品|基础")
	void SetItemOwner(ACharacterBase* ItemOwner);
public:
	UFUNCTION(BlueprintNativeEvent, Category = "物品|基础")
	void UseItemImpl(class UARPG_ItemCoreBase* ItemCore, ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const;
	virtual void UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const;

	virtual void WhenUse(ACharacterBase* ItemOwner);

	//子类使用时发生的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "WhenUse"))
	void ReceiveWhenUse(ACharacterBase* ItemOwner);

	virtual void WhenNotUse(ACharacterBase* ItemOwner);
	//子类停止使用时发生的事件
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "WhenNotUse"))
	void ReceiveWhenNotUse(ACharacterBase* ItemOwner);

public:
	UPROPERTY(EditDefaultsOnly, Category = "物品")
	UAnimMontage* UseItemMontage;
	UPROPERTY(EditDefaultsOnly, Category = "物品")
	FName UseItemAttachSocketName;

	void PlayUseItemMontage(const UARPG_ItemCoreBase* ItemCore, ACharacterBase* ItemOwner) const;
};
