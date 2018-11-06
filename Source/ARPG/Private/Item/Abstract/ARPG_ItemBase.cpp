// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemBase.h"
#include <Components/PrimitiveComponent.h>
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_CollisionType.h"
#include "ARPG_InventoryComponent.h"


#define LOCTEXT_NAMESPACE "ARPG_Item"

void AARPG_ItemBase::WhenExecuteInteract_Implementation(class ACharacterBase* InteractInvoker)
{
	InteractInvoker->Inventory->AddItemCore(GetItemCore(), GetItemCore()->Number);
	Destroy();
}

bool AARPG_ItemBase::CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const
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

class ACharacterBase* AARPG_ItemBase::GetItemOwner() const
{
	return Cast<ACharacterBase>(Instigator);
}

void AARPG_ItemBase::SetItemOwner(ACharacterBase* ItemOwner)
{
	Instigator = ItemOwner;
}

void AARPG_ItemBase::WhenUse(class ACharacterBase* ItemOwner)
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

void AARPG_ItemBase::WhenNotUse(class ACharacterBase* ItemOwner)
{
	ReceiveWhenNotUse(ItemOwner);
}

#undef LOCTEXT_NAMESPACE
