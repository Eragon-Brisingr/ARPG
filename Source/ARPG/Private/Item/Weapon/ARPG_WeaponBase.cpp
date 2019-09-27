// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WeaponBase.h"
#include <Components/PrimitiveComponent.h>
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"
#include "SocketMoveTraceManager.h"
#include "HumanBase.h"
#include "ARPG_WeaponCoreBase.h"
#include "ARPG_Item_Log.h"
#include "ARPG_Battle_Log.h"
#include "ARPG_CharacterAnimType.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

AARPG_WeaponBase::AARPG_WeaponBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void AARPG_WeaponBase::WhenUse(class ACharacterBase* ItemOwner)
{
	Item_Display_LOG("%s装备武器%s", *UXD_DebugFunctionLibrary::GetDebugName(ItemOwner), *UXD_DebugFunctionLibrary::GetDebugName(this));
	UARPG_WeaponCoreBase* WeaponCore = Cast<UARPG_WeaponCoreBase>(ItemCore);
	if (WeaponCore)
	{
		if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(GetRootComponent()))
		{
			SocketMoveTracer = NewObject<USocketMoveTracer>(this, GET_MEMBER_NAME_CHECKED(AARPG_WeaponBase, SocketMoveTracer));
			SocketMoveTracer->OnTraceActorNative.BindUObject(this, &AARPG_WeaponBase::WhenAttackTracedActor);
			SocketMoveTracer->InitSocketMoveTracer(Root, WeaponCore->SocketMoveTracerConfig);
		}


		if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
		{
			if (WeaponCore->MoveAnimInstanceOverride)
			{
				Human->SetMoveAnimInstanceOverride(WeaponCore->MoveAnimInstanceOverride);
			}
		}
	}
	Super::WhenUse(ItemOwner);
}

void AARPG_WeaponBase::WhenNotUse(class ACharacterBase* ItemOwner)
{
	UARPG_WeaponCoreBase* WeaponCore = Cast<UARPG_WeaponCoreBase>(ItemCore);
	if (WeaponCore)
	{
		if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
		{
			if (WeaponCore->MoveAnimInstanceOverride)
			{
				Human->SetMoveAnimInstanceOverride(nullptr);
			}
		}
	}

	Super::WhenNotUse(ItemOwner);
}

FVector AARPG_WeaponBase::GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const
{
	return AttackTarget->GetActorLocation();
}

FRotator AARPG_WeaponBase::GetAttackFaceRotation_Implementation(class AActor* AttackTarget) const
{
	return (AttackTarget->GetActorLocation() - GetItemOwner()->GetActorLocation()).Rotation();
}

bool AARPG_WeaponBase::IsAllowedAttack_Implementation(class AActor* AttackTarget) const
{
	return (AttackTarget->GetActorLocation() - GetItemOwner()->GetActorLocation()).Size() < 200.f;
}

void AARPG_WeaponBase::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		if (Human->LeftWeapon == this)
		{
			GetItemOwner()->ARPG_InputPressed(EARPG_InputType::LeftLightAttack);
		}
		else
		{
			GetItemOwner()->ARPG_InputPressed(EARPG_InputType::RightLightAttack);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(FinishAttack_TimeHandle, FTimerDelegate::CreateUObject(this, &AARPG_WeaponBase::FinishAttack, OnAttackFinished), 0.2f, false);
}

void AARPG_WeaponBase::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		if (Human->LeftWeapon == this)
		{
			GetItemOwner()->ARPG_InputReleased(EARPG_InputType::LeftLightAttack);
		}
		else
		{
			GetItemOwner()->ARPG_InputReleased(EARPG_InputType::RightLightAttack);
		}
	}
	OnAttackAborted.ExecuteIfBound();
}

void AARPG_WeaponBase::FinishAttack(FBP_OnAttackFinished OnAttackFinished)
{
	if (AHumanBase* Human = Cast<AHumanBase>(GetItemOwner()))
	{
		if (Human->LeftWeapon == this)
		{
			GetItemOwner()->ARPG_InputReleased(EARPG_InputType::LeftLightAttack);
		}
		else
		{
			GetItemOwner()->ARPG_InputReleased(EARPG_InputType::RightLightAttack);
		}
	}
	OnAttackFinished.ExecuteIfBound(true);
}

void AARPG_WeaponBase::EnableNearAttackTrace(const FApplyPointDamageParameter& ApplyPointDamageParameter, bool ClearIgnoreList /*= true*/)
{
	PointDamageParameter = ApplyPointDamageParameter;
	SocketMoveTracer->OnTraceActorNative.BindUObject(this, &AARPG_WeaponBase::WhenAttackTracedActor);
	SocketMoveTracer->EnableTrace(ClearIgnoreList);
}

void AARPG_WeaponBase::DisableNearAttackTrace()
{
	SocketMoveTracer->DisableTrace();
}

void AARPG_WeaponBase::WhenAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult)
{
	UARPG_WeaponCoreBase* WeaponCore = Cast<UARPG_WeaponCoreBase>(ItemCore);
	if (WeaponCore == nullptr)
	{
		return;
	}

	if (OtherActor && OtherActor != GetItemOwner())
	{
		if (ACharacterBase* WeaponOnwer = Cast<ACharacterBase>(GetItemOwner()))
		{
			Battle_Display_LOG("%s所持武器%s打击到%s", *UARPG_DebugFunctionLibrary::GetDebugName(WeaponOnwer), *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(OtherActor));

			if (ACharacterBase* ReceiveDamageCharacter = Cast<ACharacterBase>(OtherActor))
			{
				PointDamageParameter.AddHitStunValue = WeaponCore->GetHitStunValue(PointDamageParameter.AddHitStunValue);

				if (ReceiveDamageCharacter->ApplyPointDamage(WeaponCore->GetPhysicsAttackValue(), (ReceiveDamageCharacter->GetActorLocation() - TraceResult.ImpactPoint).GetSafeNormal2D(), TraceResult, WeaponOnwer, this, nullptr, PointDamageParameter) > 0.f)
				{
					WeaponOnwer->NearAttackSuccessTimeDilation(0.2f);
				}
			}
		}
	}
}

bool AARPG_WeaponBase::TraceForExecuteOther()
{
	if (UARPG_WeaponCoreBase* WeaponCore = Cast<UARPG_WeaponCoreBase>(ItemCore))
	{
		return WeaponCore->ExecuteActionSet.TraceForExecuteOther(GetItemOwner());
	}
	return false;
}

void AARPG_WeaponBase::EnableFallingAttackTrace(const FApplyPointDamageParameter& ApplyPointDamageParameter, bool ClearIgnoreList /*= true*/)
{
	PointDamageParameter = ApplyPointDamageParameter;
	SocketMoveTracer->OnTraceActorNative.BindUObject(this, &AARPG_WeaponBase::WhenFallingAttackTracedActor);
	SocketMoveTracer->EnableTrace(ClearIgnoreList);
}


void AARPG_WeaponBase::DisableFallingAttackTrace()
{
	SocketMoveTracer->DisableTrace();
}

void AARPG_WeaponBase::WhenFallingAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult)
{
	WhenAttackTracedActor(HitComponent, SocketName, OtherActor, OtherComp, TraceResult);
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

bool AARPG_WeaponBase::IsBothHandWeapon() const
{
	if (const UARPG_WeaponCoreBase* WeaponCore = Cast<const UARPG_WeaponCoreBase>(ItemCore))
	{
		return WeaponCore->IsBothHandWeapon();
	}
	return false;
}

UARPG_AttackAnimSetBase* AARPG_WeaponBase::GetAttackAnimSet() const
{
	return GetItemCoreConst()->AttackAnimSet;
}

const UARPG_WeaponCoreBase* AARPG_WeaponBase::GetItemCoreConst() const
{
	return CastChecked<const UARPG_WeaponCoreBase>(ItemCore);
}

UARPG_WeaponCoreBase* AARPG_WeaponBase::GetItemCore() const
{
	return CastChecked<UARPG_WeaponCoreBase>(ItemCore);
}

AARPG_Weapon_StaticMesh::AARPG_Weapon_StaticMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Weapon_SkeletalMesh::AARPG_Weapon_SkeletalMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}

#undef LOCTEXT_NAMESPACE
