// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemCoreBase.h"
#include "ARPG_ItemBase.h"


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


