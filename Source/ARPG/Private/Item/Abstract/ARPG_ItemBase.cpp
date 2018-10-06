// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemBase.h"


#define LOCTEXT_NAMESPACE "ARPG_Item"

FText AARPG_ItemBase::GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const
{
	return LOCTEXT("ARPG_ItemBase ItemTypeDesc", "物品");
}

#undef LOCTEXT_NAMESPACE
