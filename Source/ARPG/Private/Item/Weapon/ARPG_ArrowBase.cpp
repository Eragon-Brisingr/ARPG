// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ArrowBase.h"
#include "ARPG_ArrowCoreBase.h"
#include <AssertionMacros.h>
#include <Components/SkeletalMeshComponent.h>
#include "ARPG_CollisionType.h"
#include "CharacterBase.h"
#include "ARPG_ActorFunctionLibrary.h"


UARPG_ProjectileMovementComponent::UARPG_ProjectileMovementComponent()
	:ObjectTypes{ FARPG_CollisionObjectType::CharacterMesh }
{
	bRotationFollowsVelocity = true;
}

void UARPG_ProjectileMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	IgnoreActors.Add(GetOwner());
}

void UARPG_ProjectileMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsActive && UpdatedComponent)
	{
		FHitResult TraceResult;
		FVector End = UpdatedComponent->GetComponentTransform().TransformPosition(TraceOriginOffet);

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(UpdatedComponent, PreLocation, End, Radius, ObjectTypes, false, IgnoreActors, DrawDebugType, TraceResult, false))
		{
			if (OnTraceActorNative.IsBound())
			{
				OnTraceActorNative.Execute(UpdatedComponent, TraceResult.GetActor(), TraceResult.GetComponent(), TraceResult);
			}
		}
		PreLocation = End;
	}
}

void UARPG_ProjectileMovementComponent::Activate(bool bReset /*= false*/)
{
	if (UpdatedComponent != GetOwner()->GetRootComponent())
	{
		SetUpdatedComponent(GetOwner()->GetRootComponent());
	}
	Super::Activate(bReset);
	PreLocation = UpdatedComponent->GetComponentLocation();
}

void UARPG_ProjectileMovementComponent::Deactivate()
{
	Super::Deactivate();
}

AARPG_ArrowBase::AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_ArrowCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_ArrowBase, InnerItemCore)))
{

}

void AARPG_ArrowBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AARPG_ArrowBase::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	UARPG_ProjectileMovementComponent* ProjectileMovementComponent = FindComponentByClass<UARPG_ProjectileMovementComponent>();
	if (ProjectileMovementComponent && ProjectileMovementComponent->IsActive())
	{
		PostArrowHitOther(ProjectileMovementComponent);

		if (AARPG_ArrowBase* Arrow = Cast<AARPG_ArrowBase>(Other))
		{
			//把扎进物体里里的箭插深点
			if (AActor* ParentActor = Arrow->GetAttachParentActor())
			{
				SetItemSimulatePhysics(true);

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
			SetActorLocation(Hit.Location);
			GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
		}
	}
}

void AARPG_ArrowBase::WhenHitCharacter(USceneComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(Other))
	{
		PostArrowHitOther(FindComponentByClass<UARPG_ProjectileMovementComponent>());

		Character->ApplyPointDamage(50.f, 5.f, GetVelocity().GetSafeNormal(), Hit, GetItemOwner(), this, nullptr, nullptr);

		SetActorLocation(Hit.Location);
		GetRootComponent()->AttachToComponent(Hit.GetComponent(), FAttachmentTransformRules::KeepWorldTransform, Hit.BoneName);
	}
}

void AARPG_ArrowBase::PostArrowHitOther(UARPG_ProjectileMovementComponent* ProjectileMovementComponent)
{
	ProjectileMovementComponent->DestroyComponent();
	GetRootMeshComponent()->SetCollisionProfileName(FARPG_CollisionProfile::Item);
}

void AARPG_ArrowBase::Release(float ForceSize)
{
	UPrimitiveComponent* ArrowRootComponent = GetRootMeshComponent();
	ArrowRootComponent->SetCollisionProfileName(FARPG_CollisionProfile::ArrowReleasing);

	UARPG_ProjectileMovementComponent* ProjectileMovementComponent = UARPG_ActorFunctionLibrary::AddComponent<UARPG_ProjectileMovementComponent>(this, TEXT("弓箭移动组件"));
	ProjectileMovementComponent->IgnoreActors.Add(GetItemOwner());
	ProjectileMovementComponent->Activate(true);
	ProjectileMovementComponent->OnTraceActorNative.BindUObject(this, &AARPG_ArrowBase::WhenHitCharacter);
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ForceSize;
}
