// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EquipmentCoreBase.h"
#include "HumanBase.h"
#include "ARPG_EquipmentBase.h"
#include "ARPG_CharacterStateComponent.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

FText UARPG_EquipmentCoreBase::GetItemTypeDesc() const
{
	return LOCTEXT("ARPG_Equipment", "装备");
}

void UARPG_EquipmentCoreBase::UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput)
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

void UARPG_EquipmentCoreBase::WhenUse(ACharacterBase* ItemOwner)
{
	for (const TSubclassOf<UARPG_CharacterState_BuffBase>& BuffType : EnableBuffes)
	{
		ItemOwner->CharacterStateComponent->TryAddBuffByType(BuffType, FARPG_PropertyChangeContext(this, ItemOwner));
	}

	Super::WhenUse(ItemOwner);
}

void UARPG_EquipmentCoreBase::WhenNotUse(ACharacterBase* ItemOwner)
{
	for (const TSubclassOf<UARPG_CharacterState_BuffBase>& BuffType : EnableBuffes)
	{
		ItemOwner->CharacterStateComponent->RemoveBuffByType(BuffType);
	}

	Super::WhenNotUse(ItemOwner);
}

#undef LOCTEXT_NAMESPACE
