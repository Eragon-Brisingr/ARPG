// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ItemBase.h"
#include "ARPG_ItemBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_API AARPG_ItemBase : public AXD_ItemBase
{
	GENERATED_BODY()
public:
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
	float GetWeightImpl(const class UXD_ItemCoreBase* ItemCore) const;
	virtual float GetWeightImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const { return Weight; }

	UFUNCTION(BlueprintNativeEvent, Category = "物品|基础")
	float GetPriceImpl(const class UXD_ItemCoreBase* ItemCore) const;
	virtual float GetPriceImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const { return Price; }

	UFUNCTION(BlueprintNativeEvent, Category = "物品|基础")
	FText GetItemTypeDescImpl(const class UXD_ItemCoreBase* ItemCore) const;
	virtual FText GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const;

	
	
};
