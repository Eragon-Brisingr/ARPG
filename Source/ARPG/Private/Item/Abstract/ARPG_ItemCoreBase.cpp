// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemCoreBase.h"
#include "ARPG_ItemBase.h"
#include "CharacterBase.h"
#include "ARPG_Item_Log.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "ARPG_InventoryComponent.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

TSubclassOf<AXD_ItemBase> UARPG_ItemCoreBase::GetStaticMeshActor() const
{
	// 要求每个子道具种类自己实现自己道具类型的实例
	checkNoEntry();
	return AARPG_Item_StaticMesh::StaticClass();
}

TSubclassOf<AXD_ItemBase> UARPG_ItemCoreBase::GetSkeletalMeshActor() const
{
	// 要求每个子道具种类自己实现自己道具类型的实例
	checkNoEntry();
	return AARPG_Item_SkeletalMesh::StaticClass();
}

UARPG_ItemCoreBase::UARPG_ItemCoreBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{

}

FText UARPG_ItemCoreBase::ReceiveGetItemTypeDesc_Implementation() const
{
	return LOCTEXT("ARPG_ItemBase ItemTypeDesc", "物品");
}

float UARPG_ItemCoreBase::GetTradePrice(class UXD_InventoryComponentBase* Invoker, class UXD_InventoryComponentBase* Trader, ETradePart InvokerTradePart) const
{
	// TODO：根据关系之类的决定价格
	return GetPrice() * 1.f;
}

void UARPG_ItemCoreBase::UseItem(class ACharacterBase* ItemOwner, EUseItemInput UseItemInput)
{
	ReceiveUseItem(ItemOwner, UseItemInput);
}

void UARPG_ItemCoreBase::RemoveItemCore(ACharacterBase* ItemOwner, int32 RemoveNumber) const
{
	ItemOwner->Inventory->RemoveItemCore(this, RemoveNumber);
}

#undef LOCTEXT_NAMESPACE