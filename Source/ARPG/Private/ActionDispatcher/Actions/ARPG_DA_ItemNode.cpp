// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_DA_ItemNode.h"
#include "CharacterBase.h"
#include "ARPG_InventoryComponent.h"

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
