// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WeaponBase.h"
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"
#include "SocketMoveTraceManager.h"
#include "HumanBase.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

AARPG_WeaponBase::AARPG_WeaponBase()
{
	SocketMoveTracer = CreateDefaultSubobject<USocketMoveTracer>(GET_MEMBER_NAME_CHECKED(AARPG_WeaponBase, SocketMoveTracer));
}

void AARPG_WeaponBase::UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const
{
	ItemOwner->EquipWaepon(ItemCore, UseItemInput);
}

FText AARPG_WeaponBase::GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const
{
	return LOCTEXT("ARPG_Weapon", "武器");
}

void AARPG_WeaponBase::WhenUse(class ACharacterBase* ItemOwner)
{
	Super::WhenUse(ItemOwner);
}

void AARPG_WeaponBase::WhenNotUse(class ACharacterBase* ItemOwner)
{
	Super::WhenNotUse(ItemOwner);
}

void AARPG_WeaponBase::WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const
{
	if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
	{
		if (Human->RightWeapon && Human->RightWeapon->EqualForItemCore(ItemCore))
		{
			if (Human->LeftWeapon && Human->LeftWeapon->EqualForItemCore(ItemCore))
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
		else if (ExistNumber == 0 && Human->LeftWeapon && Human->LeftWeapon->EqualForItemCore(ItemCore))
		{
			Human->SetLeftWeapon(nullptr);
		}
	}
}

void AARPG_WeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SocketMoveTracer->InitSocketMoveTracer(GetRootMeshComponent());
}

void AARPG_WeaponBase::SetEnableNearAttackTrace(bool Enable)
{
	SocketMoveTracer->SetEnableTrace(Enable);
}

void AARPG_WeaponBase::WhenInHand()
{
	ReceiveWhenInHand();
}

void AARPG_WeaponBase::WhenInWeaponBack()
{
	ReceiveWhenInWeaponBack();
}

void AARPG_WeaponBase::AttachWeaponTo(class USceneComponent* InParent, FName InSocketName)
{
	AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
}

#undef LOCTEXT_NAMESPACE
