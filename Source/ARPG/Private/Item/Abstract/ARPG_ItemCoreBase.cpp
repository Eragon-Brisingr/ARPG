// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemCoreBase.h"
#include "ARPG_ItemBase.h"


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

void UARPG_ItemCoreBase::UseItem(class APawn* ItemOwner, EUseItemInput UseItemInput)
{
	GetItemDefaultActor<AARPG_ItemBase>()->UseItemImpl(this, ItemOwner, UseItemInput);
}

