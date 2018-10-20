// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ProjectileMovementComponent.h"
#include "ARPG_CollisionType.h"
#include <GameFramework/Actor.h>


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


