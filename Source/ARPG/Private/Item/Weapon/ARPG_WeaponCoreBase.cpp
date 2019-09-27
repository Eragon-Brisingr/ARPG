// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WeaponCoreBase.h"
#include "ARPG_WeaponBase.h"
#include "HumanBase.h"
#include "ARPG_CharacterAnimType.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

UARPG_WeaponCoreBase::UARPG_WeaponCoreBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{
	AttackAnimSet = CreateDefaultSubobject<UARPG_AttackAnimSetNormal>(GET_MEMBER_NAME_CHECKED(UARPG_WeaponCoreBase, AttackAnimSet));
}

FText UARPG_WeaponCoreBase::GetItemTypeDesc_Implementation() const
{
	return LOCTEXT("ARPG_WeaponType", "武器");
}

void UARPG_WeaponCoreBase::UseItem_Implementation(ACharacterBase* ItemOwner, EUseItemInput UseItemInput)
{
	ItemOwner->EquipWaepon(this, UseItemInput);
}

void UARPG_WeaponCoreBase::WhenRemoveFromInventory(class AActor* ItemOwner, int32 RemoveNumber, int32 ExistNumber)
{
	if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
	{
		if (Human->RightWeapon && Human->RightWeapon->IsEqualWithItemCore(this))
		{
			if (Human->LeftWeapon && Human->LeftWeapon->IsEqualWithItemCore(this))
			{
				if (ExistNumber == 1)
				{
					Human->SetLeftWeapon(nullptr);
				}
			}
			if (ExistNumber <= 0)
			{
				Human->SetRightWeapon(nullptr);
			}
		}
		else if (ExistNumber == 0 && Human->LeftWeapon && Human->LeftWeapon->IsEqualWithItemCore(this))
		{
			Human->SetLeftWeapon(nullptr);
		}
	}
}

TSubclassOf<AXD_ItemBase> UARPG_WeaponCoreBase::GetStaticMeshActor() const
{
	return AARPG_Weapon_StaticMesh::StaticClass();
}

TSubclassOf<AXD_ItemBase> UARPG_WeaponCoreBase::GetSkeletalMeshActor() const
{
	return AARPG_Weapon_SkeletalMesh::StaticClass();
}

bool UARPG_WeaponCoreBase::IsBothHandWeapon() const
{
	switch (WeaponUseType)
	{
	case EWeaponUseType::BothHand:
	case EWeaponUseType::BothHandForLeft:
	case EWeaponUseType::BothHandForRight:
		return true;
	default:
		return false;
	}
}

#undef LOCTEXT_NAMESPACE
