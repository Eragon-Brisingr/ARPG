// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanBase.h"
#include "ARPG_MovementComponent.h"
#include "ARPG_WeaponBase.h"
#include <UnrealNetwork.h>
#include "ARPG_ItemCoreBase.h"
#include "ARPG_InventoryComponent.h"

AHumanBase::AHumanBase(const FObjectInitializer& PCIP)
	:Super(PCIP)
{
	ARPG_MovementComponent->MovementState.bCanCrouch = true;
	ARPG_MovementComponent->SetIsReplicated(true);

	{
#if WITH_EDITOR
		DefaultLeftWeapon.bShowNumber = false;
		DefaultLeftWeapon.ShowItemType = AARPG_WeaponBase::StaticClass();
		DefaultRightWeapon.bShowNumber = false;
		DefaultRightWeapon.ShowItemType = AARPG_WeaponBase::StaticClass();
#endif
	}
}

void AHumanBase::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHumanBase, LeftWeapon);
	DOREPLIFETIME(AHumanBase, RightWeapon);

	DOREPLIFETIME(AHumanBase, UseWeaponState);
}

void AHumanBase::WhenGameInit_Implementation()
{
	Super::WhenGameInit_Implementation();

	if (UARPG_ItemCoreBase* ItemCore = Cast<UARPG_ItemCoreBase>(DefaultLeftWeapon.ItemCore))
	{
		UseItemImmediately(ItemCore, EUseItemInput::LeftMouse);
	}

	if (UARPG_ItemCoreBase* ItemCore = Cast<UARPG_ItemCoreBase>(DefaultRightWeapon.ItemCore))
	{
		UseItemImmediately(ItemCore, EUseItemInput::RightMouse);
	}
}

TArray<struct FXD_Item> AHumanBase::GetInitItemList() const
{
	TArray<FXD_Item> Res = Super::GetInitItemList();
	if (DefaultLeftWeapon)
	{
		Res.Add(DefaultLeftWeapon);
	}
	if (DefaultRightWeapon)
	{
		Res.Add(DefaultRightWeapon);
	}
	if (DefaultArrow)
	{
		Res.Add(DefaultArrow);
	}
	return Res;
}

class AARPG_WeaponBase* AHumanBase::EquipWaepon_Implementation(class UARPG_ItemCoreBase* WeaponCore, EUseItemInput UseItemInput)
{
	const AARPG_WeaponBase* Weapon = WeaponCore->GetItemDefaultActor<AARPG_WeaponBase>();
	switch (Weapon->WeaponUseType)
	{
	//左手双手武器写这，卸下右手武器，别忘了写卸下左手双手武器代码
	case EWeaponUseType::BothHandForLeft:
		if (RightWeapon)
		{
			SetRightWeapon(nullptr);
		}
	case EWeaponUseType::SingleHandForLeft:
		return EquipSingleLeftWeapon(WeaponCore);
	//右手双持武器逻辑写这，卸下左手武器，别忘了写卸下右手双手武器代码
	case EWeaponUseType::BothHandForRight:
		if (LeftWeapon)
		{
			SetLeftWeapon(nullptr);
		}
	case EWeaponUseType::SingleHandForRight:
		return EquipSingleRightWeapon(WeaponCore);
	//单手武器逻辑
	default:
		switch (UseItemInput)
		{
		case EUseItemInput::LeftMouse:
			if (RightWeapon && RightWeapon->EqualForItemCore(WeaponCore))
			{
				if (Inventory->GetItemNumber(RightWeapon) == 1)
				{
					SetRightWeapon(nullptr);
				}
			}
			return EquipSingleLeftWeapon(WeaponCore);
		case EUseItemInput::RightMouse:
			if (LeftWeapon && LeftWeapon->EqualForItemCore(WeaponCore))
			{
				if (Inventory->GetItemNumber(LeftWeapon) == 1)
				{
					SetLeftWeapon(nullptr);
				}
			}
			return EquipSingleRightWeapon(WeaponCore);
		default:
			break;
		}
		break;
	}
	return nullptr;
}

void AHumanBase::OnRep_EquipVariable(class AARPG_ItemBase* CurEquip, class AARPG_ItemBase* PreEquip)
{
	if (PreEquip)
	{
		PreEquip->WhenNotUse(this);
		OnNotEquip.Broadcast(this, PreEquip);
	}
	if (CurEquip)
	{
		CurEquip->WhenUse(this);
		OnEquip.Broadcast(this, CurEquip);
	}
}

void AHumanBase::SetLeftWeapon(class AARPG_WeaponBase* ToLeftWeapon)
{
	SetEquipVariable(LeftWeapon, ToLeftWeapon);
}

void AHumanBase::OnRep_LeftWeapon(class AARPG_WeaponBase* PreLeftWeapon)
{
	OnRep_EquipVariable(LeftWeapon, PreLeftWeapon);
}

void AHumanBase::SetRightWeapon(class AARPG_WeaponBase* ToRightWeapon)
{
	SetEquipVariable(RightWeapon, ToRightWeapon);
}

void AHumanBase::OnRep_RightWeapon(class AARPG_WeaponBase* PreRightWeapon)
{
	OnRep_EquipVariable(RightWeapon, PreRightWeapon);
}

class AARPG_WeaponBase* AHumanBase::EquipSingleRightWeapon(class UARPG_ItemCoreBase* WeaponCore)
{
	if (RightWeapon)
	{
		if (RightWeapon->EqualForItemCore(WeaponCore))
		{
			SetRightWeapon(nullptr);
			if (LeftWeapon == nullptr)
			{
				UseWeaponState = EUseWeaponState::NoneWeapon_Default;
			}
			return nullptr;
		}
		else
		{
			//把原来手上的武器卸下
			SetRightWeapon(nullptr);
		}
	}
	if (LeftWeapon)
	{
		switch (LeftWeapon->WeaponUseType)
		{
			//卸下左手双手武器
		case EWeaponUseType::BothHandForLeft:
			SetLeftWeapon(nullptr);
		}
	}
	//Spawn武器并装备
	SetRightWeapon(Cast<AARPG_WeaponBase>(WeaponCore->SpawnItemActorForOwner(this, this)));
	if (UseWeaponState == EUseWeaponState::NoneWeapon_Default)
	{
		RightWeaponInWeaponBack();
	}
	else
	{
		//WhenPullOutWeapon();
	}

	return RightWeapon;
}

class AARPG_WeaponBase* AHumanBase::EquipSingleLeftWeapon(class UARPG_ItemCoreBase* WeaponCore)
{
	if (LeftWeapon)
	{
		if (LeftWeapon->EqualForItemCore(WeaponCore))
		{
			SetLeftWeapon(nullptr);
			if (RightWeapon == nullptr)
			{
				UseWeaponState = EUseWeaponState::NoneWeapon_Default;
			}
			return nullptr;
		}
		else
		{
			//把原来手上的武器卸下
			SetLeftWeapon(nullptr);
		}
	}
	if (RightWeapon)
	{
		switch (RightWeapon->WeaponUseType)
		{
			//卸下右手双手武器
		case EWeaponUseType::BothHandForRight:
			SetRightWeapon(nullptr);
		}
	}
	//Spawn武器并装备
	SetLeftWeapon(Cast<AARPG_WeaponBase>(WeaponCore->SpawnItemActorForOwner(this, this)));
	if (UseWeaponState == EUseWeaponState::NoneWeapon_Default)
	{
		LeftWeaponInWeaponBack();
	}
	else
	{
		//WhenPullOutWeapon();
	}

	return LeftWeapon;
}

void AHumanBase::RightWeaponInHand()
{
	RightWeapon->AttachWeaponTo(GetMesh(), RightWeapon->RightWeaponInHandSocket);
	RightWeapon->WhenInHand();
}

void AHumanBase::LeftWeaponInHand()
{
	LeftWeapon->AttachWeaponTo(GetMesh(), LeftWeapon->LeftWeaponInHandSocket);
	LeftWeapon->WhenInHand();
}

void AHumanBase::RightWeaponInWeaponBack()
{
	RightWeapon->AttachWeaponTo(GetMesh(), RightWeapon->RightWeaponInWeaponBackSocket);
	RightWeapon->WhenInWeaponBack();
}

void AHumanBase::LeftWeaponInWeaponBack()
{
	LeftWeapon->AttachWeaponTo(GetMesh(), LeftWeapon->LeftWeaponInWeaponBackSocket);
	LeftWeapon->WhenInWeaponBack();
}

void AHumanBase::LetTheWeaponInHand()
{
	UseWeaponState = EUseWeaponState::UsingWeapon_Defalut;
	if (LeftWeapon)
	{
		LeftWeaponInHand();
	}
	if (RightWeapon)
	{
		RightWeaponInHand();
	}
}

void AHumanBase::LetTheWeaponInWeaponBack()
{
	UseWeaponState = EUseWeaponState::NoneWeapon_Default;
	if (LeftWeapon)
	{
		LeftWeaponInWeaponBack();
	}
	if (RightWeapon)
	{
		RightWeaponInWeaponBack();
	}
}

void AHumanBase::InvokePullOutWeapon()
{
	if (UseWeaponState == EUseWeaponState::NoneWeapon_Default && CanPlayFullBodyMontage())
	{
		PlayMontage(PullOutWeaponMontage);
	}
}

void AHumanBase::InvokeTakeBackWeapon()
{
	if (UseWeaponState == EUseWeaponState::UsingWeapon_Defalut && CanPlayFullBodyMontage())
	{
		PlayMontage(TakeBackWeaponMontage);
	}
}
