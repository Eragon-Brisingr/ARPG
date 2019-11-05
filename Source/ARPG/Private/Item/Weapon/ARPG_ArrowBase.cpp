// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ArrowBase.h"
#include "ARPG_ArrowCoreBase.h"
#include <AssertionMacros.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>
#include "ARPG_CollisionType.h"
#include "CharacterBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "HumanBase.h"
#include "ARPG_ProjectileMovementComponent.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "ARPG_Battle_Log.h"



AARPG_ArrowBase::AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	
}

void AARPG_ArrowBase::WhenHitCharacter(USceneComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit, FApplyPointDamageParameter ApplyPointDamageParameter)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(Other))
	{
		PostArrowHitOther(FindComponentByClass<UARPG_ProjectileMovementComponent>());

		if (ApplyDamamgeToCharacter(Character, Hit, ApplyPointDamageParameter) > 0.f)
		{
			SetActorLocation(Hit.Location);
			GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
		}
	}
}

float AARPG_ArrowBase::ApplyDamamgeToCharacter(ACharacterBase* Character, const FHitResult& Hit, const FApplyPointDamageParameter& ApplyPointDamageParameter)
{
	Battle_Display_LOG("%s的弓%s所射箭%s的射中%s", *UARPG_DebugFunctionLibrary::GetDebugName(GetItemOwner()), *UARPG_DebugFunctionLibrary::GetDebugName(GetOwner()), *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(Character));

	if (UARPG_ArrowCoreBase* ArrowCore = Cast<UARPG_ArrowCoreBase>(ItemCore))
	{
		FApplyPointDamageParameter Param(ApplyPointDamageParameter);
		Param.AddHitStunValue = ArrowCore->GetHitStunValue(ApplyPointDamageParameter.AddHitStunValue);
		return Character->ApplyPointDamage(GetArrowDamage(), GetVelocity().GetSafeNormal(), Hit, FARPG_PropertyChangeContext(GetItemOwner(), this), nullptr, Param);
	}
	return 0.f;
}

void AARPG_ArrowBase::WhenArrowHitEnvironment(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (UARPG_ProjectileMovementComponent* ProjectileMovementComponent = FindComponentByClass<UARPG_ProjectileMovementComponent>())
	{
		PostArrowHitOther(ProjectileMovementComponent);

		if (AARPG_ArrowBase* Arrow = Cast<AARPG_ArrowBase>(OtherActor))
		{
			//把扎进物体里里的箭插深点
			if (AActor* ParentActor = Arrow->GetAttachParentActor())
			{
				SetItemSimulatePhysics(true);

				FVector Velocity = GetVelocity();
				Arrow->AddActorWorldOffset(Velocity.GetSafeNormal() * FMath::GetMappedRangeValueClamped({ 0.f, 3000.f }, { 0.f, 50.f }, Velocity.Size()));

				// （扎在角色身上的箭不能与别的物体碰撞比较合理）TODO：考虑箭击中扎在角色身上的箭造成伤害？
// 				if (HasAuthority())
// 				{
// 					if (ACharacterBase* Character = Cast<ACharacterBase>(ParentActor))
// 					{
// 						FHitResult ArrowTraceRes;
// 						bool TraceSucceed = Character->ActorLineTraceSingle(ArrowTraceRes, Arrow->GetActorLocation(), Arrow->GetActorLocation() + Arrow->GetActorForwardVector() * 100.f, ECC_Visibility, FCollisionQueryParams(NAME_None, true));
// 
// 						ApplyDamamgeToCharacter(Character, TraceSucceed ? ArrowTraceRes : Hit);
// 					}
// 				}
			}
		}
		else
		{
			//射到别的Item上的情况
			if (AARPG_ItemBase* Item = Cast<AARPG_ItemBase>(OtherActor))
			{
				if (ACharacterBase* Character = Item->GetItemOwner())
				{
					//射中防御状态中人的持的武器
					if (HasAuthority())
					{
						Battle_Display_LOG("%s的弓%s所射箭%s的射到防御中的%s的%s上", *UARPG_DebugFunctionLibrary::GetDebugName(GetItemOwner()), *UARPG_DebugFunctionLibrary::GetDebugName(GetOwner()), *UARPG_DebugFunctionLibrary::GetDebugName(this), *UARPG_DebugFunctionLibrary::GetDebugName(Character), *UARPG_DebugFunctionLibrary::GetDebugName(Item));
						Character->WhenDefenseSucceed(GetArrowDamage(), GetItemOwner(), GetVelocity(), 20.f, Hit);
					}

					SetItemSimulatePhysics(true);
				}
			}
			else
			{
				SetActorLocation(Hit.Location);
				GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
			}
		}
	}
}

float AARPG_ArrowBase::GetArrowDamage()
{
	// TODO：实现弓箭伤害
	return 50.f;
}

void AARPG_ArrowBase::PostArrowHitOther(UARPG_ProjectileMovementComponent* ProjectileMovementComponent)
{
	ProjectileMovementComponent->DestroyComponent();
	SetItemCollisionProfileName(FARPG_CollisionProfile::Item);
	GetRootMeshComponent()->OnComponentHit.RemoveDynamic(this, &AARPG_ArrowBase::WhenArrowHitEnvironment);
	SetOwner(nullptr);
	SetItemOwner(nullptr);
}

void AARPG_ArrowBase::Launch(float ForceSize, const FApplyPointDamageParameter& ApplyPointDamageParameter)
{
	Launch_Multicast(ForceSize);
	if (HasAuthority())
	{
		UARPG_ProjectileMovementComponent* ProjectileMovementComponent = FindComponentByClass<UARPG_ProjectileMovementComponent>();
		ProjectileMovementComponent->OnTraceActorNative.BindUObject(this, &AARPG_ArrowBase::WhenHitCharacter, ApplyPointDamageParameter);
	}
}

void AARPG_ArrowBase::Launch_Multicast_Implementation(float ForceSize)
{

	if (UPrimitiveComponent* ArrowRootComponent = GetRootMeshComponent())
	{
		SetItemCollisionProfileName(FARPG_CollisionProfile::ArrowReleasing);

		UARPG_ProjectileMovementComponent* ProjectileMovementComponent = UARPG_ActorMoveUtils::AddComponent<UARPG_ProjectileMovementComponent>(this, TEXT("弓箭移动组件"));
		ProjectileMovementComponent->IgnoreActors.Add(GetItemOwner());
		ProjectileMovementComponent->Activate(true);
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ForceSize;
		ArrowRootComponent->OnComponentHit.AddDynamic(this, &AARPG_ArrowBase::WhenArrowHitEnvironment);
	}
}

void AARPG_ArrowBase::ClientArrowStop()
{
	if (UARPG_ProjectileMovementComponent* ProjectileMovementComponent = FindComponentByClass<UARPG_ProjectileMovementComponent>())
	{
		PostArrowHitOther(ProjectileMovementComponent);
	}
}

void AARPG_ArrowBase::OnRep_AttachmentReplication()
{
	Super::OnRep_AttachmentReplication();

	if (GetAttachmentReplication().AttachParent)
	{
		ClientArrowStop();
	}
}

void AARPG_ArrowBase::WhenUse(class ACharacterBase* ItemOwner)
{
	Super::WhenUse(ItemOwner);

	// 显示箭袋
	UARPG_ArrowCoreBase* ArrowCore = Cast<UARPG_ArrowCoreBase>(ItemCore);
	if (ArrowCore && !ArrowCore->Dorlach.IsNull())
	{
		// TODO：异步加载
		UStaticMesh* Dorlach = ArrowCore->Dorlach.LoadSynchronous();
		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(GetRootComponent()))
		{
			StaticMeshComponent->SetStaticMesh(Dorlach);
		}
		//太Hack了
		ArrowCore->ItemMesh = Dorlach;
	}
}

const UARPG_ArrowCoreBase* AARPG_ArrowBase::GetItemCoreConst() const
{
	return CastChecked<const UARPG_ArrowCoreBase>(ItemCore);
}

UARPG_ArrowCoreBase* AARPG_ArrowBase::GetItemCore() const
{
	return CastChecked<UARPG_ArrowCoreBase>(ItemCore);
}

AARPG_Arrow_StaticMesh::AARPG_Arrow_StaticMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Arrow_SkeletalMesh::AARPG_Arrow_SkeletalMesh(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AXD_Item_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}
