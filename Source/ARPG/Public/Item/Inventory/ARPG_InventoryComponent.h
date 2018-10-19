// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/XD_InventoryComponentBase.h"
#include "ARPG_InventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_InventoryComponent : public UXD_InventoryComponentBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "背包")
	void AddItemArray(const TArray<FARPG_Item>& Items);

	//金钱相关
public:
	bool TradeItem(int32 DeductMoney, UARPG_InventoryComponent* OtherInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number);

	//考虑网络的Owner
	bool BuyItemFromOther(UARPG_InventoryComponent* OtherInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1);
	bool SellItemToOther(UARPG_InventoryComponent* OtherInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number = 1);

	UFUNCTION(BlueprintCallable, Category = "背包")
	bool SpendMoney(int32 SpandStandardMoney, TArray<class UARPG_ItemCoreBase*>& SpendedMoneys);

	UFUNCTION(BlueprintPure, Category = "背包")
	float GetStandardMoneyNumber() const;

	UFUNCTION(BlueprintCallable, Category = "背包")
	const TArray<UARPG_ItemCoreBase*>& GetItemCoreList() const;
};
