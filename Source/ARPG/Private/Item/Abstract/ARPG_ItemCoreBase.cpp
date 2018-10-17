// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemCoreBase.h"
#include "ARPG_ItemBase.h"
#include "CharacterBase.h"
#include "ARPG_Item_Log.h"
#include "ARPG_DebugFunctionLibrary.h"


UARPG_ItemCoreBase::UARPG_ItemCoreBase()
{
	ItemClass = AARPG_ItemBase::StaticClass();
}

float UARPG_ItemCoreBase::GetWeight() const
{
	return GetItemDefaultActor<AARPG_ItemBase>()->GetWeightImpl(this);
}

float UARPG_ItemCoreBase::GetPrice() const
{
	return GetItemDefaultActor<AARPG_ItemBase>()->GetPriceImpl(this);
}

FText UARPG_ItemCoreBase::GetItemTypeDesc() const
{
	return GetItemDefaultActor<AARPG_ItemBase>()->GetItemTypeDescImpl(this);
}

float UARPG_ItemCoreBase::GetTradePrice(class UXD_InventoryComponentBase* Invoker, class UXD_InventoryComponentBase* Trader, ETradePart InvokerTradePart) const
{
	return GetPrice() * 1.f;
}

FText UARPG_ItemCoreBase::GetDescribe() const
{
	return GetItemDefaultActor<AARPG_ItemBase>()->Describe;
}

void UARPG_ItemCoreBase::UseItem(class ACharacterBase* ItemOwner, EUseItemInput UseItemInput)
{
	if (ItemOwner)
	{
		Item_Display_LOG("%s使用道具%s", *UARPG_DebugFunctionLibrary::GetDebugName(ItemOwner), *UARPG_DebugFunctionLibrary::GetDebugName(GetItemDefaultActor()));
		GetItemDefaultActor<AARPG_ItemBase>()->UseItemImpl(this, ItemOwner, UseItemInput);
	}
}

