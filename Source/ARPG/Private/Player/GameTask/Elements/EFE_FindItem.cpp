// Fill out your copyright notice in the Description page of Project Settings.

#include "EFE_FindItem.h"
#include <UnrealNetwork.h>
#include "CharacterBase.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_ItemBase.h"

void UEFE_FindItemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEFE_FindItemBase, TargetNumber, COND_InitialOnly);
	DOREPLIFETIME(UEFE_FindItemBase, CurrentNumber);
}

void UEFE_FindItemBase::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	ACharacterBase* Character = GetARPG_Character();
	Character->Inventory->OnAddItem.AddDynamic(this, &UEFE_FindItemBase::WhenAddItem);
	Character->Inventory->OnRemoveItem.AddDynamic(this, &UEFE_FindItemBase::WhenRemoveItem);
	
	for (UXD_ItemCoreBase* ItemCore : Character->Inventory->ItemCoreList)
	{
		if (IsNeedFindItem(ItemCore))
		{
			CurrentNumber += ItemCore->Number;
		}
	}
	if (CurrentNumber >= TargetNumber)
	{
		FinishEventFlowElement();
	}
}

void UEFE_FindItemBase::WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	ACharacterBase* Character = GetARPG_Character();
	Character->Inventory->OnAddItem.RemoveDynamic(this, &UEFE_FindItemBase::WhenAddItem);
	Character->Inventory->OnRemoveItem.RemoveDynamic(this, &UEFE_FindItemBase::WhenRemoveItem);
}

bool UEFE_FindItemBase::IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const
{
	unimplemented();
	return false;
}

void UEFE_FindItemBase::WhenAddItem(UXD_ItemCoreBase* ItemCore, int32 AddNumber, int32 ExistNumber)
{
	if (IsNeedFindItem(ItemCore))
	{
		CurrentNumber += AddNumber;
		if (IsFinished() == false && CurrentNumber >= TargetNumber)
		{
			FinishEventFlowElement();
		}
	}
}

void UEFE_FindItemBase::WhenRemoveItem(UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber)
{
	if (IsNeedFindItem(ItemCore))
	{
		CurrentNumber -= RemoveNumber;
		if (IsFinished() == true && CurrentNumber < TargetNumber)
		{
			SetUnfinished();
		}
	}
}

UEFE_FindItem_ByRef::UEFE_FindItem_ByRef()
{
	TargetItem.bShowNumber = false;
}

void UEFE_FindItem_ByRef::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEFE_FindItem_ByRef, TargetItem, COND_InitialOnly);
}

bool UEFE_FindItem_ByRef::IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const
{
	return TargetItem->IsEqualWithItemCore(ItemCore);
}

void UEFE_FindItem_ByType::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UEFE_FindItem_ByType, TargetItemType, COND_InitialOnly);
}

bool UEFE_FindItem_ByType::IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const
{
	return TargetItemType && ItemCore->ItemClass->IsChildOf(TargetItemType);
}
