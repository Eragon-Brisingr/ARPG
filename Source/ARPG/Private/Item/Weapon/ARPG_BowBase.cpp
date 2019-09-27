// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BowBase.h"
#include <Components/SkeletalMeshComponent.h>
#include "Components/StaticMeshComponent.h"
#include "ARPG_BowCoreBase.h"
#include "ARPG_ArrowCoreBase.h"
#include "HumanBase.h"
#include "ARPG_ArrowBase.h"
#include "ARPG_InventoryComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"



AARPG_BowBase::AARPG_BowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{

}

void AARPG_BowBase::SpawnArrowInHand()
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		if (Human->Arrow)
		{
			HoldingArrow = Cast<AARPG_ArrowBase>(Human->Arrow->GetItemCoreConst()->SpawnItemActorForOwner(this, Human));
			HoldingArrow->SetItemSimulatePhysics(false);
			HoldingArrow->AttachWeaponTo(Human->GetMesh(), Human->LeftWeapon == this ? Human->LeftHandHoldingArrowSocketName : Human->RightHandHoldingArrowSocketName);
		}
	}
}

void AARPG_BowBase::LaunchArrow(float FullBowTime, const FApplyPointDamageParameter& ApplyPointDamageParameter)
{
	if (HoldingArrow)
	{
		if (HasAuthority())
		{
			if (ACharacterBase* Character = GetItemOwner())
			{
				Character->Inventory->RemoveItemCore(HoldingArrow->GetItemCoreConst());
			}
		}

		HoldingArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HoldingArrow->Launch(FMath::GetMappedRangeValueClamped({ 0.f, FullBowTime }, { 500.f, 3000.f }, HoldingTime), ApplyPointDamageParameter);

		HoldingArrow = nullptr;
	}
}

bool AARPG_BowBase::IsAllowedAttack_Implementation(class AActor* AttackTarget) const
{
	return true;
}

FVector AARPG_BowBase::GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const
{
	FVector MoveToLocation = AttackTarget->GetActorLocation() + (GetItemOwner()->GetActorLocation() - AttackTarget->GetActorLocation()).GetSafeNormal() * 500.f;
	return MoveToLocation;
}

void AARPG_BowBase::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		Human->ARPG_InputPressed(EARPG_InputType::LeftLightAttack);
		Human->EnableAutoUpdateLookAtRotation(false);
		GetWorld()->GetTimerManager().SetTimer(BowRelease_TimerHandle, FTimerDelegate::CreateUObject(this, &AARPG_BowBase::AI_ReleaseArrow, OnAttackFinished), 4.f, false);
	}
}

void AARPG_BowBase::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		if (APawn* AttackTargetPawn = Cast<APawn>(AttackTarget))
		{
			Human->SetLookAtLocation(AttackTargetPawn->GetPawnViewLocation());
		}
	}
}

void AARPG_BowBase::AI_ReleaseArrow(FBP_OnAttackFinished OnAttackFinished)
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		Human->ARPG_InputReleased(EARPG_InputType::LeftLightAttack);
		Human->EnableAutoUpdateLookAtRotation(true);
		OnAttackFinished.ExecuteIfBound(true);
	}
}

AARPG_Bow_StaticMesh::AARPG_Bow_StaticMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Bow_SkeletalMesh::AARPG_Bow_SkeletalMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}
