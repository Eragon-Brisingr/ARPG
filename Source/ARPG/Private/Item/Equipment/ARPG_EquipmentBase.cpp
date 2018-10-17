// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EquipmentBase.h"
#include "CharacterBase.h"
#include "HumanBase.h"
#include "ARPG_EquipmentCoreBase.h"
#include "ARPG_Item_Log.h"
#include "ARPG_DebugFunctionLibrary.h"
#include <Components/SkeletalMeshComponent.h>


#define LOCTEXT_NAMESPACE "ARPG_Item"

AARPG_EquipmentBase::AARPG_EquipmentBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_EquipmentCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_EquipmentBase, InnerItemCore)))
{
	
}

void AARPG_EquipmentBase::UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const
{
	ItemOwner->EquipEquipment(CastChecked<UARPG_EquipmentCoreBase>(ItemCore), UseItemInput);
}

FText AARPG_EquipmentBase::GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const
{
	return LOCTEXT("ARPG_Equipment", "装备");
}

void AARPG_EquipmentBase::WhenUse(class ACharacterBase* ItemOwner)
{
	Item_Display_LOG("%s装备装备%s", *UARPG_DebugFunctionLibrary::GetDebugName(ItemOwner), *UARPG_DebugFunctionLibrary::GetDebugName(this));
	Super::WhenUse(ItemOwner);
	
	AttachToComponent(ItemOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipSocketName);
	if (EquipSocketName.IsNone())
	{
		if (USkeletalMeshComponent* SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetRootComponent()))
		{
			SkeletalMeshComponent->SetMasterPoseComponent(ItemOwner->GetMesh());
		}
	}
}

void AARPG_EquipmentBase::WhenNotUse(class ACharacterBase* ItemOwner)
{
	Super::WhenNotUse(ItemOwner);
}

void AARPG_EquipmentBase::WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const
{
	if (ExistNumber <= 0)
	{
		if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
		{
			int32 FindIndex = Human->EquipmentList.IndexOfByPredicate([ItemCore](AARPG_EquipmentBase* E_Equipment) {return E_Equipment->EqualForItemCore(ItemCore); });
			if (FindIndex != INDEX_NONE)
			{
				Human->EquipmentList.RemoveAt(FindIndex);
				Human->OnRep_EquipmentList();
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
