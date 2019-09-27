// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemBase.h"
#include <Components/PrimitiveComponent.h>
#include "Components/StaticMeshComponent.h"
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_CollisionType.h"
#include "ARPG_InventoryComponent.h"
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_Item_Log.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

void AARPG_ItemBase::WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker)
{
	InteractInvoker->Inventory->AddItemCore(GetItemCoreConst(), GetItemCoreConst()->Number);
	InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Succeed);
	Destroy();
}

bool AARPG_ItemBase::CanInteract_Implementation(const ACharacterBase* InteractInvoker) const
{
	return GetItemOwner() == nullptr;
}

#if WITH_EDITOR
void AARPG_ItemBase::EditorReplacedActor(AActor* OldActor)
{
	Super::EditorReplacedActor(OldActor);

	if (OldActor->HasAnyFlags(RF_Transient))
	{
		SetFlags(RF_Transient);
	}
}
#endif

AARPG_ItemBase::AARPG_ItemBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

float AARPG_ItemBase::GetWeight() const
{
	if (UARPG_ItemCoreBase* ARPG_ItemCore = Cast<UARPG_ItemCoreBase>(ItemCore))
	{
		return ARPG_ItemCore->GetWeight();
	}
	return 0.f;
}

float AARPG_ItemBase::GetPrice() const
{
	if (UARPG_ItemCoreBase* ARPG_ItemCore = Cast<UARPG_ItemCoreBase>(ItemCore))
	{
		return ARPG_ItemCore->GetPrice();
	}
	return 0.f;
}

FText AARPG_ItemBase::GetItemTypeDesc() const
{
	if (UARPG_ItemCoreBase* ARPG_ItemCore = Cast<UARPG_ItemCoreBase>(ItemCore))
	{
		return ARPG_ItemCore->GetItemTypeDesc();
	}
	return FText::GetEmpty();
}

FText AARPG_ItemBase::GetDescribe() const
{
	if (UARPG_ItemCoreBase* ARPG_ItemCore = Cast<UARPG_ItemCoreBase>(ItemCore))
	{
		return ARPG_ItemCore->GetDescribe();
	}
	return FText::GetEmpty();
}

ACharacterBase* AARPG_ItemBase::GetItemOwner() const
{
	ACharacterBase* ItemOwner = Cast<ACharacterBase>(Instigator);
	return ItemOwner ? ItemOwner : Cast<ACharacterBase>(GetOwner());
}

void AARPG_ItemBase::SetItemOwner(ACharacterBase* ItemOwner)
{
	Instigator = ItemOwner;
}

void AARPG_ItemBase::WhenUse(ACharacterBase* ItemOwner)
{
	GetItemCore()->WhenUse(ItemOwner);
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
	GetItemCore()->WhenNotUse(ItemOwner);
	ReceiveWhenNotUse(ItemOwner);
}

const UARPG_ItemCoreBase* AARPG_ItemBase::GetItemCoreConst() const
{
	return CastChecked<const UARPG_ItemCoreBase>(ItemCore);
}

UARPG_ItemCoreBase* AARPG_ItemBase::GetItemCore() const
{
	return CastChecked<UARPG_ItemCoreBase>(ItemCore);
}

#if WITH_EDITOR
bool AARPG_ItemBase::IsSelectable() const
{
	return !HasAnyFlags(RF_Transient);
}
#endif

AARPG_Item_StaticMesh::AARPG_Item_StaticMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Item_SkeletalMesh::AARPG_Item_SkeletalMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}

#undef LOCTEXT_NAMESPACE
