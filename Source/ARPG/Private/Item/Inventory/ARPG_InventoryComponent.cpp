// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_MoneyCoreBase.h"
#include "CharacterBase.h"
#include "XD_TemplateLibrary.h"


#define LOCTEXT_NAMESPACE "ARPG_Item"

UARPG_InventoryComponent::UARPG_InventoryComponent()
{

}

void UARPG_InventoryComponent::AddItemArray(const TArray<UARPG_ItemCoreBase*>& Items)
{
	UXD_InventoryComponentBase::AddItemArray(ArrayCast<UXD_ItemCoreBase*>(Items));
}

bool UARPG_InventoryComponent::TradeItem(int32 DeductMoney, UARPG_InventoryComponent* OtherInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number)
{
	TArray<class UARPG_ItemCoreBase*> SpendedMoneys;
	if (SpendMoney(DeductMoney, SpendedMoneys))
	{
		for (UARPG_ItemCoreBase* ElementMoney : SpendedMoneys)
		{
			OtherInventory->AddItemCore(ElementMoney, ElementMoney->Number);
		}
		int32 AddNumber = OtherInventory->RemoveItemCore(ItemCore, Number);
		if (AddNumber > 0)
		{
			AddItemCore(ItemCore, AddNumber);
			OnGetItemFromOther.Broadcast(GetOwner(), ItemCore, AddNumber, true);
			OtherInventory->OnRemoveItemByOther.Broadcast(GetOwner(), ItemCore, AddNumber, true);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool UARPG_InventoryComponent::BuyItemFromOther(UARPG_InventoryComponent* OtherInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	int32 DeductMoney = ItemCore->GetTradePrice(this, OtherInventory, ETradePart::Buyer) * Number;
	return TradeItem(DeductMoney, OtherInventory, ItemCore, Number);
}

bool UARPG_InventoryComponent::SellItemToOther(UARPG_InventoryComponent* OtherInventory, class UARPG_ItemCoreBase* ItemCore, int32 Number /*= 1*/)
{
	int32 DeductMoney = ItemCore->GetTradePrice(this, OtherInventory, ETradePart::Seller) * Number;
	return OtherInventory->TradeItem(DeductMoney, this, ItemCore, Number);
}

bool UARPG_InventoryComponent::SpendMoney(int32 SpandStandardMoney, TArray<class UARPG_ItemCoreBase*>& SpendedMoneys)
{
	if (GetStandardMoneyNumber() >= SpandStandardMoney)
	{
		while (SpandStandardMoney > 0)
		{
			//TODO: 游戏性相关，应该允许金钱使用的排序策略
			for (UXD_ItemCoreBase* ItemCore : ItemCoreList)
			{
				if (UARPG_MoneyCoreBase* Money = Cast<UARPG_MoneyCoreBase>(ItemCore))
				{
					int32 Temp = SpandStandardMoney;
					SpandStandardMoney -= ItemCore->Number * Money->GetPrice();
					if (SpandStandardMoney > 0)
					{
						Temp = ItemCore->Number * Money->GetPrice();
					}
					SpendedMoneys.Add(Money);
					RemoveItemCore(ItemCore, FMath::CeilToInt(Temp / Money->GetPrice()));
					break;
				}
			}
		}
		return true;
	}
	return false;
}

float UARPG_InventoryComponent::GetStandardMoneyNumber() const
{
	float StandardMoney = 0.f;
	for (UXD_ItemCoreBase* ItemCore : ItemCoreList)
	{
		if (const UARPG_MoneyCoreBase* Money = Cast<UARPG_MoneyCoreBase>(ItemCore))
		{
			StandardMoney += ItemCore->Number * Money->GetPrice();
		}
	}
	return StandardMoney;
}

const TArray<UARPG_ItemCoreBase*>& UARPG_InventoryComponent::GetItemCoreList() const
{
	return ArrayCast<UARPG_ItemCoreBase*>(ItemCoreList);
}

#undef LOCTEXT_NAMESPACE
