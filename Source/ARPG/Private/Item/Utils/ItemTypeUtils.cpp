// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemTypeUtils.h"
#include "ARPG_ItemBase.h"
#include "ARPG_ItemCoreBase.h"




FARPG_Item::FARPG_Item()
{
#if WITH_EDITOR
	ShowItemType = AARPG_ItemBase::StaticClass();
#endif
}
