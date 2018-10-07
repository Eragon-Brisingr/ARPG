// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WeaponBase.h"
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"

void AARPG_WeaponBase::UseItemImpl_Implementation(class UXD_ItemCoreBase* ItemCore, class APawn* ItemOwner, EUseItemInput UseItemInput) const
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(ItemOwner))
	{
		if (UARPG_ItemCoreBase* WeaponCore = Cast<UARPG_ItemCoreBase>(ItemCore))
		{
			Character->EquipWaepon(WeaponCore, UseItemInput);
		}
	}
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
