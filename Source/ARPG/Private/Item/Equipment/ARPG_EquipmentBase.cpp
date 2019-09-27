// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EquipmentBase.h"
#include "CharacterBase.h"
#include "HumanBase.h"
#include "ARPG_EquipmentCoreBase.h"
#include "ARPG_Item_Log.h"
#include "ARPG_DebugFunctionLibrary.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>


#define LOCTEXT_NAMESPACE "ARPG_Item"

AARPG_EquipmentBase::AARPG_EquipmentBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

#if WITH_EDITOR
void AARPG_EquipmentBase::EditorReplacedActor(AActor* OldActor)
{
	Super::EditorReplacedActor(OldActor);
	AARPG_EquipmentBase* OldEquipment = Cast<AARPG_EquipmentBase>(OldActor);
	if (ACharacterBase* ItemOwner = OldEquipment->GetItemOwner())
	{
		InitItemMesh();
		if (USkeletalMeshComponent * SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetRootComponent()))
		{
			SkeletalMeshComponent->SetMasterPoseComponent(ItemOwner->GetMesh());
		}
	}
}
#endif

void AARPG_EquipmentBase::WhenUse(class ACharacterBase* ItemOwner)
{
	Item_Display_LOG("%s装备装备%s", *UARPG_DebugFunctionLibrary::GetDebugName(ItemOwner), *UARPG_DebugFunctionLibrary::GetDebugName(this));
	Super::WhenUse(ItemOwner);
	
	if (UARPG_EquipmentCoreBase * EquipmentCore = Cast<UARPG_EquipmentCoreBase>(ItemCore))
	{
		AttachToComponent(ItemOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquipmentCore->EquipSocketName);
		if (USkeletalMeshComponent * SkeletalMeshComponent = Cast<USkeletalMeshComponent>(GetRootComponent()))
		{
			SkeletalMeshComponent->SetMasterPoseComponent(ItemOwner->GetMesh());
		}

		if (AHumanBase * Human = Cast<AHumanBase>(ItemOwner))
		{
			if (GetItemCoreConst()->bHideShorts)
			{
				Human->SetHideShorts(true);
			}
			if (GetItemCoreConst()->bHideUnderwear)
			{
				Human->SetHideUnderwear(true);
			}
		}
	}
}

void AARPG_EquipmentBase::WhenNotUse(class ACharacterBase* ItemOwner)
{
	Super::WhenNotUse(ItemOwner);

	if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
	{
		if (GetItemCoreConst()->bHideShorts)
		{
			Human->SetHideShorts(false);
		}
		if (GetItemCoreConst()->bHideUnderwear)
		{
			Human->SetHideUnderwear(false);
		}
	}
}

const UARPG_EquipmentCoreBase* AARPG_EquipmentBase::GetItemCoreConst() const
{
	return CastChecked<UARPG_EquipmentCoreBase>(ItemCore);
}

AARPG_Equipment_StaticMesh::AARPG_Equipment_StaticMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Equipment_SkeletalMesh::AARPG_Equipment_SkeletalMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}

#undef LOCTEXT_NAMESPACE
