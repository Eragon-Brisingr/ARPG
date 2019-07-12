// Fill out your copyright notice in the Description page of Project Settings.

#include "EFE_FindItem.h"
#include <UnrealNetwork.h>
#include "CharacterBase.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_ItemBase.h"
#include <Engine/ActorChannel.h>

#define LOCTEXT_NAMESPACE "ARPG_EventFlow"

void UEFE_FindItemBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_FindItemBase, TargetNumber);
	DOREPLIFETIME(UEFE_FindItemBase, CurrentNumber);
}

void UEFE_FindItemBase::WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner)
{
	CurrentNumber = 0;

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
#if WITH_EDITOR
	TargetItem.bShowNumber = false;
#endif
}

void UEFE_FindItem_ByRef::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_FindItem_ByRef, TargetItem);
}

void UEFE_FindItem_ByRef::ReplicatedElementSubobject(bool& WroteSomething, class UActorChannel * Channel, class FOutBunch * Bunch, FReplicationFlags * RepFlags)
{
	if (TargetItem)
	{
		WroteSomething |= Channel->ReplicateSubobject(TargetItem.ItemCore, *Bunch, *RepFlags);
	}
}

bool UEFE_FindItem_ByRef::IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const
{
	return TargetItem->IsEqualWithItemCore(ItemCore);
}

EEventFlowCompileMessageType UEFE_FindItem_ByRef::GetCompileMessage_Implementation(FString& Message) const
{
	if (TargetItem.ItemCore == nullptr)
	{
		Message = TEXT("目标物品不得为空");
		return EEventFlowCompileMessageType::Error;
	}
	return EEventFlowCompileMessageType::None;
}

FText UEFE_FindItem_ByRef::ReceiveGetDescribe_Implementation() const
{
	if (TargetItem.ItemCore == nullptr)
	{
		return FText::GetEmpty();
	}

	if (TargetNumber == 1)
	{
		return FText::Format(LOCTEXT("需找到物品{0}", "需找到物品{0}"), TargetItem->GetItemName());
	}
	else
	{
		return FText::Format(LOCTEXT("需找到物品{0} {1}/{2}", "需找到物品{0} {1}/{2}"), TargetItem->GetItemName(), FMath::Min(CurrentNumber, TargetNumber), TargetNumber);
	}
}

void UEFE_FindItem_ByType::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEFE_FindItem_ByType, TargetItemType);
}

bool UEFE_FindItem_ByType::IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const
{
	return TargetItemType && ItemCore->GetItemClass()->IsChildOf(TargetItemType);
}

EEventFlowCompileMessageType UEFE_FindItem_ByType::GetCompileMessage_Implementation(FString& Message) const
{
	if (TargetItemType == nullptr)
	{
		Message = TEXT("目标物品类型不得为空");
		return EEventFlowCompileMessageType::Error;
	}
	return EEventFlowCompileMessageType::None;
}

FText UEFE_FindItem_ByType::ReceiveGetDescribe_Implementation() const
{
	if (TargetItemType == nullptr)
	{
		return FText::GetEmpty();
	}

	if (TargetNumber == 1)
	{
		return FText::Format(LOCTEXT("需找到物品{0}", "需找到物品{0}"), TargetItemType.GetDefaultObject()->GetItemName());
	}
	else
	{
		return FText::Format(LOCTEXT("需找到物品{0} {1}/{2}", "需找到物品{0} {1}/{2}"), TargetItemType.GetDefaultObject()->GetItemName(), FMath::Min(CurrentNumber, TargetNumber), TargetNumber);
	}
}

#undef LOCTEXT_NAMESPACE
