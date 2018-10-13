// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemBase.h"
#include <Components/PrimitiveComponent.h>
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"


#define LOCTEXT_NAMESPACE "ARPG_Item"

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
	return CastChecked<ACharacterBase>(Instigator);
}

void AARPG_ItemBase::WhenUse(class ACharacterBase* ItemOwner)
{
	bNetUseOwnerRelevancy = true;
	SetOwner(ItemOwner);

	SetActorEnableCollision(false);
	SetItemSimulatePhysics(false);

	ReceiveWhenUse(ItemOwner);
}

void AARPG_ItemBase::WhenNotUse(class ACharacterBase* ItemOwner)
{
	ReceiveWhenNotUse(ItemOwner);
}

void AARPG_ItemBase::SetItemSimulatePhysics(bool bSimulate)
{
	if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(RootComponent))
	{
		Root->SetSimulatePhysics(bSimulate);
	}
}

#undef LOCTEXT_NAMESPACE
