// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BowBase.h"
#include <Components/SkeletalMeshComponent.h>
#include "ARPG_BowCoreBase.h"
#include "HumanBase.h"
#include "ARPG_ArrowBase.h"



AARPG_BowBase::AARPG_BowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_BowCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_BowBase, InnerItemCore)))
{

}

void AARPG_BowBase::SpawnArrowInHand()
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		if (Human->Arrow)
		{
			HoldingArrow = Cast<AARPG_ArrowBase>(Human->Arrow->GetItemCore()->SpawnItemActorForOwner(this, Human));
			HoldingArrow->SetItemSimulatePhysics(false);
			HoldingArrow->AttachWeaponTo(Human->GetMesh(), Human->HoldingArrowSocketName);
		}
	}
}

void AARPG_BowBase::LaunchArrow()
{
	if (HoldingArrow)
	{
		HoldingArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingArrow->Launch(3000.f);

		HoldingArrow = nullptr;
	}
}
