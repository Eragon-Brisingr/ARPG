// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ArrowBase.h"
#include "ARPG_ArrowCoreBase.h"
#include <AssertionMacros.h>
#include <Components/SkeletalMeshComponent.h>
#include "ARPG_CollisionType.h"
#include "CharacterBase.h"


UProjectileTracer::UProjectileTracer()
	:ObjectTypes{ FARPG_CollisionObjectType::CharacterMesh },
	bRotationUseVelocity(true)
{

}

void UProjectileTracer::Tick(float DeltaTime)
{
	if (TracedComponent.IsValid())
	{
		FHitResult TraceResult;
		FVector End = TracedComponent->GetComponentTransform().TransformPosition(TraceOriginOffet);

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(TracedComponent.Get(), PreLocation, End, Radius, ObjectTypes, false, IgnoreActors, DrawDebugType, TraceResult, false))
		{
			if (OnTraceActorNative.IsBound())
			{
				OnTraceActorNative.Execute(TracedComponent.Get(), TraceResult.GetActor(), TraceResult.GetComponent(), TraceResult);
			}
		}

		if (TracedComponent->GetComponentVelocity().Size() > 0.f)
		{
			TracedComponent->SetWorldRotation(TracedComponent->GetComponentVelocity().Rotation());
		}
		PreLocation = End;
	}
}

void UProjectileTracer::SetTraceEnable(bool Enable)
{
	if (TracedComponent.IsValid() && bTraceable != Enable)
	{
		bTraceable = Enable;
		if (Enable)
		{
			PreLocation = TracedComponent->GetComponentLocation();
		}
	}
}

void UProjectileTracer::SetTargetComponent(class UPrimitiveComponent* TargetComponent)
{
	if (TargetComponent != nullptr && TargetComponent != TracedComponent)
	{
		TracedComponent = TargetComponent;
		IgnoreActors.Add(TargetComponent->GetOwner());
	}
}

AARPG_ArrowBase::AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_ArrowCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_ArrowBase, InnerItemCore)))
{
	ProjectileTracer = CreateDefaultSubobject<UProjectileTracer>(GET_MEMBER_NAME_CHECKED(AARPG_ArrowBase, ProjectileTracer));
}

void AARPG_ArrowBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ProjectileTracer->IgnoreActors.Add(GetItemOwner());
	ProjectileTracer->TracedComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	ProjectileTracer->OnTraceActorNative.BindUObject(this, &AARPG_ArrowBase::WhenHitCharacter);
}

void AARPG_ArrowBase::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ProjectileTracer && ProjectileTracer->bTraceable)
	{
		PostArrowHitOther();

		if (AARPG_ArrowBase* Arrow = Cast<AARPG_ArrowBase>(Other))
		{
			//把扎进物体里里的箭插深点
			if (AActor* ParentActor = Arrow->GetAttachParentActor())
			{
				FVector Velocity = GetVelocity();
				Arrow->AddActorWorldOffset(Velocity.GetSafeNormal() * FMath::GetMappedRangeValueClamped({ 0.f, 3000.f }, { 0.f, 50.f }, Velocity.Size()));

				//假如扎在角色身上，则造成伤害
				if (ACharacterBase* Character = Cast<ACharacterBase>(ParentActor))
				{
					FHitResult ArrowTraceRes;
					bool TraceSucceed = Character->ActorLineTraceSingle(ArrowTraceRes, Arrow->GetActorLocation(), Arrow->GetActorLocation() + Arrow->GetActorForwardVector() * 100.f, ECC_Visibility, FCollisionQueryParams(NAME_None, true));
					Character->ApplyPointDamage(50.f, 5.f, GetVelocity().GetSafeNormal(), TraceSucceed ? ArrowTraceRes : Hit, GetItemOwner(), this, nullptr, nullptr);
				}
			}
		}
		else
		{
			SetItemSimulatePhysics(false);
			SetActorLocation(Hit.Location);
			GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
		}
	}
}

void AARPG_ArrowBase::WhenHitCharacter(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(Other))
	{
		PostArrowHitOther();

		Character->ApplyPointDamage(50.f, 5.f, GetVelocity().GetSafeNormal(), Hit, GetItemOwner(), this, nullptr, nullptr);

		SetItemSimulatePhysics(false);
		SetActorLocation(Hit.Location);
		GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
	}
}

void AARPG_ArrowBase::PostArrowHitOther()
{
	ProjectileTracer->SetTraceEnable(false);
	GetRootMeshComponent()->SetCollisionProfileName(FARPG_CollisionProfile::Item);
}

void AARPG_ArrowBase::Release(float ForceSize)
{
	SetItemSimulatePhysics(true);
	UPrimitiveComponent* ArrowRootComponent = GetRootMeshComponent();
	ArrowRootComponent->SetCollisionProfileName(FARPG_CollisionProfile::ArrowReleasing);
	ArrowRootComponent->AddForce(GetActorForwardVector() * ForceSize);
	ProjectileTracer->SetTraceEnable(true);
}
