// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EquipmentCoreBase.h"
#include "HumanBase.h"
#include "ARPG_EquipmentBase.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

FText UARPG_EquipmentCoreBase::GetItemTypeDesc_Implementation() const
{
	return LOCTEXT("ARPG_Equipment", "装备");
}

void UARPG_EquipmentCoreBase::UseItem_Implementation(ACharacterBase* ItemOwner, EUseItemInput UseItemInput)
{
	ItemOwner->EquipEquipment(this, UseItemInput);
}

void UARPG_EquipmentCoreBase::WhenRemoveFromInventory(class AActor* ItemOwner, int32 RemoveNumber, int32 ExistNumber)
{
	if (ExistNumber <= 0)
	{
		if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
		{
			int32 FindIndex = Human->EquipmentList.IndexOfByPredicate([this](AARPG_EquipmentBase* E_Equipment) {return E_Equipment->IsEqualWithItemCore(this); });
			if (FindIndex != INDEX_NONE)
			{
				Human->EquipmentList.RemoveAt(FindIndex);
				Human->OnRep_EquipmentList();
			}
		}
	}
}

TSubclassOf<AXD_ItemBase> UARPG_EquipmentCoreBase::GetStaticMeshActor() const
{
	return AARPG_Equipment_StaticMesh::StaticClass();
}

TSubclassOf<AXD_ItemBase> UARPG_EquipmentCoreBase::GetSkeletalMeshActor() const
{
	return AARPG_Equipment_SkeletalMesh::StaticClass();
}

#undef LOCTEXT_NAMESPACE
