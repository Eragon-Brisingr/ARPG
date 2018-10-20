// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ArrowBase.h"
#include "ARPG_ArrowCoreBase.h"
#include <AssertionMacros.h>
#include <Components/SkeletalMeshComponent.h>
#include "ARPG_CollisionType.h"
#include "CharacterBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "HumanBase.h"
#include "ARPG_ProjectileMovementComponent.h"



AARPG_ArrowBase::AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_ArrowCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_ArrowBase, InnerItemCore)))
{

}

void AARPG_ArrowBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AARPG_ArrowBase::UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const
{
	if (UARPG_ArrowCoreBase* ArrowCore = Cast<UARPG_ArrowCoreBase>(ItemCore))
	{
		ItemOwner->EquipArrow(ArrowCore, UseItemInput);
	}
}

void AARPG_ArrowBase::WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const
{
	if (ExistNumber <= 0)
	{
		if (AHumanBase* Human = Cast<AHumanBase>(ItemOwner))
		{
			Human->SetArrow(nullptr);
		}
	}
}

void AARPG_ArrowBase::WhenHitCharacter(USceneComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if (ACharacterBase* Character = Cast<ACharacterBase>(Other))
		{
			PostArrowHitOther(FindComponentByClass<UARPG_ProjectileMovementComponent>());

			Character->ApplyPointDamage(50.f, 5.f, GetVelocity().GetSafeNormal(), Hit, GetItemOwner(), this, nullptr, nullptr);

			SetActorLocation(Hit.Location);
			GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
		}
	}
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

				//假如扎在角色身上，则造成伤害
				if (HasAuthority())
				{
					if (ACharacterBase* Character = Cast<ACharacterBase>(ParentActor))
					{
						FHitResult ArrowTraceRes;
						bool TraceSucceed = Character->ActorLineTraceSingle(ArrowTraceRes, Arrow->GetActorLocation(), Arrow->GetActorLocation() + Arrow->GetActorForwardVector() * 100.f, ECC_Visibility, FCollisionQueryParams(NAME_None, true));
						Character->ApplyPointDamage(50.f, 5.f, GetVelocity().GetSafeNormal(), TraceSucceed ? ArrowTraceRes : Hit, GetItemOwner(), this, nullptr, nullptr);
					}
				}
			}
		}
		else
		{
			SetActorLocation(Hit.Location);
			GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
		}
	}
}

void AARPG_ArrowBase::PostArrowHitOther(UARPG_ProjectileMovementComponent* ProjectileMovementComponent)
{
	ProjectileMovementComponent->DestroyComponent();
	SetItemCollisionProfileName(FARPG_CollisionProfile::Item);
	GetRootMeshComponent()->OnComponentHit.RemoveDynamic(this, &AARPG_ArrowBase::WhenArrowHitEnvironment);
}

void AARPG_ArrowBase::Launch_Implementation(float ForceSize)
{
	if (UPrimitiveComponent* ArrowRootComponent = GetRootMeshComponent())
	{
		SetItemCollisionProfileName(FARPG_CollisionProfile::ArrowReleasing);

		UARPG_ProjectileMovementComponent* ProjectileMovementComponent = UARPG_ActorFunctionLibrary::AddComponent<UARPG_ProjectileMovementComponent>(this, TEXT("弓箭移动组件"));
		ProjectileMovementComponent->IgnoreActors.Add(GetItemOwner());
		ProjectileMovementComponent->Activate(true);
		ProjectileMovementComponent->OnTraceActorNative.BindUObject(this, &AARPG_ArrowBase::WhenHitCharacter);

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
