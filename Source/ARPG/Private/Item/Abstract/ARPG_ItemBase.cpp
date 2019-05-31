// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemBase.h"
#include <Components/PrimitiveComponent.h>
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_CollisionType.h"
#include "ARPG_InventoryComponent.h"
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_Item_Log.h"


#define LOCTEXT_NAMESPACE "ARPG_Item"

void AARPG_ItemBase::WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker)
{
	InteractInvoker->Inventory->AddItemCore(GetItemCore(), GetItemCore()->Number);
	InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Succeed);
	Destroy();
}

bool AARPG_ItemBase::CanInteract_Implementation(const ACharacterBase* InteractInvoker) const
{
	return GetItemOwner() == nullptr;
}

AARPG_ItemBase::AARPG_ItemBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_ItemCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_ItemBase, InnerItemCore)))
{
	
}

FText AARPG_ItemBase::GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const
{
	return LOCTEXT("ARPG_ItemBase ItemTypeDesc", "物品");
}

ACharacterBase* AARPG_ItemBase::GetItemOwner() const
{
	return Cast<ACharacterBase>(Instigator);
}

void AARPG_ItemBase::SetItemOwner(ACharacterBase* ItemOwner)
{
	Instigator = ItemOwner;
}

void AARPG_ItemBase::UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const
{

}

void AARPG_ItemBase::WhenUse(ACharacterBase* ItemOwner)
{
	bNetUseOwnerRelevancy = true;
	SetOwner(ItemOwner);
	SetItemOwner(ItemOwner);

	if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		Root->SetCollisionResponseToChannel(FARPG_ECollisionChannel::AI_Sight, ECollisionResponse::ECR_Ignore);
	}
	SetActorEnableCollision(false);
	SetItemSimulatePhysics(false);

	ReceiveWhenUse(ItemOwner);
}

void AARPG_ItemBase::WhenNotUse(ACharacterBase* ItemOwner)
{
	ReceiveWhenNotUse(ItemOwner);
}

void AARPG_ItemBase::PlayUseItemMontage(const UARPG_ItemCoreBase* ItemCore, ACharacterBase* ItemOwner) const
{
	AARPG_ItemBase* PendingUseItem = CastChecked<AARPG_ItemBase>(ItemCore->SpawnItemActorForOwner(ItemOwner, ItemOwner));
	PendingUseItem->SetItemSimulatePhysics(false);
	PendingUseItem->SetActorHiddenInGame(true);
	PendingUseItem->AttachToComponent(ItemOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, UseItemAttachSocketName);
	ItemOwner->PendingUseItem = PendingUseItem;
	ItemOwner->PlayMontageWithBlendingOutDelegate(UseItemMontage, FOnMontageBlendingOutStarted::CreateWeakLambda(ItemOwner, [=](UAnimMontage* Montage, bool bInterrupted)
		{
			if (ItemOwner->PendingUseItem)
			{
				if (bInterrupted)
				{
					ItemOwner->PendingUseItem->Destroy();
				}
				else
				{
					Item_Warning_LOG("%s的动画%s已经播放完，但是角色还引用该道具，执行销毁", *UXD_DebugFunctionLibrary::GetDebugName(this), *UXD_DebugFunctionLibrary::GetDebugName(Montage));
					ItemOwner->PendingUseItem->Destroy();
				}
			}
		}), {}, 1.f, NAME_None, false);
}

#undef LOCTEXT_NAMESPACE
