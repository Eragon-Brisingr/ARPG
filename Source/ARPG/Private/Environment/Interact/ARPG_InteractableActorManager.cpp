// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActorManager.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_CharacterBehaviorBase.h"

// Sets default values for this component's properties
UARPG_InteractableActorManagerBase::UARPG_InteractableActorManagerBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UARPG_InteractableActorManagerBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARPG_InteractableActorManagerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARPG_InteractableActorManagerBase::StartInteract(ACharacterBase* Invoker, const FOnInteractFinished& OnInteractFinished)
{
	if (CanInteract(Invoker))
	{
		FVector InteractableLocation;
		FRotator InteractableRotation;
		GetInteractableLocationAndRotation(Invoker, InteractableLocation, InteractableRotation);
		UARPG_MoveUtils::ARPG_MoveToLocation(Invoker, InteractableLocation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenMoveFinished, Invoker, InteractableLocation, InteractableRotation, OnInteractFinished));
	}
	else
	{
		OnInteractFinished.ExecuteIfBound(false);
	}
}

void UARPG_InteractableActorManagerBase::WhenMoveFinished(const FPathFollowingResult& Result, ACharacterBase* Invoker, FVector Location, FRotator Rotation, FOnInteractFinished OnInteractFinished)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		if (CanInteract(Invoker))
		{
			GetBehavior()->ExecuteBehavior(Invoker, Location, Rotation, OnInteractFinished);
			WhenBeginInteract(Invoker);
			OnBeginInteract.Broadcast(GetOwner(), this, Invoker);
		}
		else
		{
			OnInteractFinished.ExecuteIfBound(false);
		}
	}
	else
	{
		OnInteractFinished.ExecuteIfBound(false);
	}
}

void UARPG_InteractableActorManagerBase::EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished)
{
	GetBehavior()->AbortBehavior(Invoker, UARPG_CharacterBehaviorBase::FOnBehaviorAbortFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenBehaviorAbortFinished, Invoker, OnInteractAbortFinished));
}

void UARPG_InteractableActorManagerBase::WhenBehaviorAbortFinished(ACharacterBase* Invoker, FOnInteractAbortFinished OnInteractAbortFinished)
{
	WhenEndInteract(Invoker);
	OnEndInteract.Broadcast(GetOwner(), this, Invoker);
	OnInteractAbortFinished.ExecuteIfBound();
}

void UARPG_InteractableActorManagerBase::GetInteractableLocationAndRotation(ACharacterBase* Invoker, FVector& Location, FRotator& Rotation) const
{
	Location = GetOwner()->GetActorLocation();
	Rotation = GetOwner()->GetActorRotation();
}

void UARPG_InteractableActorManagerSample::GetInteractableLocationAndRotation(ACharacterBase* Invoker, FVector& Location, FRotator& Rotation) const
{
	Location = GetOwner()->GetActorTransform().TransformPosition(Position.GetLocation());
	Rotation = GetOwner()->GetActorTransform().TransformRotation(Position.GetRotation()).Rotator();
}
