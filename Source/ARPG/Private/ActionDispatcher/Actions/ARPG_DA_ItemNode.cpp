// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_DA_ItemNode.h"
#include "CharacterBase.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"

TArray<AActor*> UARPG_DA_MoveItem::GetAllRegistableEntities() const
{
	return { Role.Get(), Other.Get() };
}

void UARPG_DA_MoveItem::WhenActionActived()
{
	if (UARPG_InventoryComponent* Inventory = Other->FindComponentByClass<UARPG_InventoryComponent>())
	{
		Role->OpenMoveItemPanel(Inventory);
	}
	Role->OnCloseMoveItemPanelNative.AddWeakLambda(this, [=]()
		{
			Role->OnCloseMoveItemPanelNative.RemoveAll(this);
			ExecuteEventAndFinishAction(WhenMoveItemFinished);
		});
}

void UARPG_DA_MoveItem::WhenActionDeactived()
{
	Role->CloseMoveItemPanel();
}

void UARPG_DA_MoveItem::WhenActionFinished()
{

}

void UARPG_DA_MoveItem_Record::WhenActionActived()
{
	Super::WhenActionActived();

	Role->Inventory->OnAddItem.AddDynamic(this, &UARPG_DA_MoveItem_Record::WhenAddItem);
	Role->Inventory->OnRemoveItem.AddDynamic(this, &UARPG_DA_MoveItem_Record::WhenAddItem);
}

void UARPG_DA_MoveItem_Record::WhenActionDeactived()
{
	Super::WhenActionDeactived();

	Role->Inventory->OnAddItem.RemoveAll(this);
	Role->Inventory->OnRemoveItem.RemoveAll(this);
}

void UARPG_DA_MoveItem_Record::WhenAddItem(UXD_ItemCoreBase* ItemCore, int32 AddNumber, int32 ExistNumber)
{
	int32 Idx = RoleAddedItems.IndexOfByPredicate([&](UXD_ItemCoreBase* E) {return E->IsEqualWithItemCore(ItemCore); });
	if (Idx == INDEX_NONE)
	{
		UARPG_ItemCoreBase* AddedItemCore = ItemCore->DeepDuplicateCore<UARPG_ItemCoreBase>(this);
		AddedItemCore->Number = AddNumber;
		RoleAddedItems.Add(AddedItemCore);
	}
	else
	{
		RoleAddedItems[Idx]->Number += AddNumber;
	}
}

void UARPG_DA_MoveItem_Record::WhenRemoveItem(UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber)
{
	int32 Idx = RoleRemovedItems.IndexOfByPredicate([&](UXD_ItemCoreBase* E) {return E->IsEqualWithItemCore(ItemCore); });
	if (Idx == INDEX_NONE)
	{
		UARPG_ItemCoreBase* RemovedItemCore = ItemCore->DeepDuplicateCore<UARPG_ItemCoreBase>(this);
		RemovedItemCore->Number = RemoveNumber;
		RoleRemovedItems.Add(RemovedItemCore);
	}
	else
	{
		RoleRemovedItems[Idx]->Number += RemoveNumber;
		if (RoleRemovedItems[Idx]->Number <= 0)
		{
			RoleRemovedItems.RemoveAt(Idx);
		}
	}
}

TArray<AActor*> UARPG_DA_TradeItem::GetAllRegistableEntities() const
{
	return { Role.Get(), Other.Get() };
}

void UARPG_DA_TradeItem::WhenActionActived()
{
	if (UARPG_InventoryComponent * Inventory = Other->FindComponentByClass<UARPG_InventoryComponent>())
	{
		Role->OpenTradeItemPanel(Inventory);
	}
	Role->OnCloseTradeItemPanelNative.AddWeakLambda(this, [=]()
		{
			Role->OnCloseTradeItemPanelNative.RemoveAll(this);
			ExecuteEventAndFinishAction(WhenTradeItemFinished);
		});
}

void UARPG_DA_TradeItem::WhenActionDeactived()
{
	Role->CloseTradeItemPanel();
}

void UARPG_DA_TradeItem::WhenActionFinished()
{

}
