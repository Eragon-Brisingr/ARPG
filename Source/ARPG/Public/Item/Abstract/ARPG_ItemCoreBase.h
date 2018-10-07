// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ItemCoreBase.h"
#include "ItemTypeUtils.h"
#include "ARPG_ItemCoreBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_ItemCoreBase : public UXD_ItemCoreBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetWeight() const;

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetPrice() const;

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	FText GetItemTypeDesc() const;
	
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	FText GetDescribe() const;
	
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetTradePrice(class UXD_InventoryComponentBase* Invoker, class UXD_InventoryComponentBase* Trader, ETradePart InvokerTradePart) const;

public:
	UFUNCTION(BlueprintCallable, Category = "物品|基础")
	void UseItem(class APawn* ItemOwner, EUseItemInput UseItemInput);
};
