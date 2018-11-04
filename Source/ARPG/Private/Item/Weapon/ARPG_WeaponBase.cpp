// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_WeaponBase.h"
#include <Components/PrimitiveComponent.h>
#include "CharacterBase.h"
#include "ARPG_ItemCoreBase.h"
#include "SocketMoveTraceManager.h"
#include "HumanBase.h"
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_WeaponCoreBase.h"
#include "ARPG_Item_Log.h"
#include "ARPG_Battle_Log.h"
#include "ARPG_CharacterAnimType.h"
#include "ARPG_ActorFunctionLibrary.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

AARPG_WeaponBase::AARPG_WeaponBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_WeaponCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_WeaponBase, InnerItemCore)))
{
	AttackAnimSet = CreateDefaultSubobject<UARPG_AttackAnimSetNormal>(GET_MEMBER_NAME_CHECKED(AARPG_WeaponBase, AttackAnimSet));
}

void AARPG_WeaponBase::UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const
{
	ItemOwner->EquipWaepon(CastChecked<UARPG_WeaponCoreBase>(ItemCore), UseItemInput);
}

FText AARPG_WeaponBase::GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const
{
	return LOCTEXT("ARPG_Weapon", "武器");
}

void AARPG_WeaponBase::WhenUse(class ACharacterBase* ItemOwner)
{
	Item_Display_LOG("%s装备武器%s", *UXD_DebugFunctionLibrary::GetDebugName(ItemOwner), *UXD_DebugFunctionLibrary::GetDebugName(this));
	if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		SocketMoveTracer = NewObject<USocketMoveTracer>(this, GET_MEMBER_NAME_CHECKED(AARPG_WeaponBase, SocketMoveTracer));
		SocketMoveTracer->OnTraceActorNative.BindUObject(this, &AARPG_WeaponBase::WhenAttackTracedActor);
		SocketMoveTracer->InitSocketMoveTracer(Root, SocketMoveTracerConfig);
	}
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
}

void AARPG_WeaponBase::SetEnableNearAttackTrace(bool Enable, bool ClearIgnoreList /*= true*/)
{
	if (Enable)
	{
		SocketMoveTracer->OnTraceActorNative.BindUObject(this, &AARPG_WeaponBase::WhenAttackTracedActor);
		SocketMoveTracer->EnableTrace(ClearIgnoreList);
	}
	else
	{
		SocketMoveTracer->DisableTrace();
	}
}

void AARPG_WeaponBase::WhenAttackTracedActor(UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult)
{
	if (OtherActor && OtherActor != GetItemOwner())
	{
		if (ACharacterBase* WeaponOnwer = Cast<ACharacterBase>(GetItemOwner()))
		{
			Battle_Display_LOG("%s所持武器打击到%s", *UXD_DebugFunctionLibrary::GetDebugName(WeaponOnwer), *UXD_DebugFunctionLibrary::GetDebugName(OtherActor));

			if (ACharacterBase* ReceiveDamageCharacter = Cast<ACharacterBase>(OtherActor))
			{
				if (ReceiveDamageCharacter->ApplyPointDamage(GetPhysicsAttackValue(), GetHitStunValue(), (ReceiveDamageCharacter->GetActorLocation() - TraceResult.ImpactPoint).GetSafeNormal2D(), TraceResult, WeaponOnwer, this, nullptr, ReceiveDamageAction) > 0.f)
				{
					WeaponOnwer->NearAttackSuccessTimeDilation(0.2f);

					//击退效果
					FVector BeakBackOffset = (ReceiveDamageCharacter->GetActorLocation() - WeaponOnwer->GetActorLocation()).GetSafeNormal2D() * BeakBackDistance;
					UARPG_ActorFunctionLibrary::PushActorTo(ReceiveDamageCharacter, BeakBackOffset);
				}
			}
		}
	}
}

bool AARPG_WeaponBase::TraceForExecuteOther()
{
	return ExecuteActionSet.TraceForExecuteOther(GetItemOwner());
}

void AARPG_WeaponBase::SetEnableFallingAttackTrace(bool Enable, bool ClearIgnoreList /*= true*/)
{
	if (Enable)
	{
		SocketMoveTracer->OnTraceActorNative.BindUObject(this, &AARPG_WeaponBase::WhenFallingAttackTracedActor);
		SocketMoveTracer->EnableTrace(ClearIgnoreList);
	}
	else
	{
		SocketMoveTracer->DisableTrace();
	}
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

#undef LOCTEXT_NAMESPACE
