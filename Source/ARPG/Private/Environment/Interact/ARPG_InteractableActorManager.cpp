// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActorManager.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_ActorFunctionLibrary.h"

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
		UARPG_MoveUtils::ARPG_MoveToLocation(Invoker, InteractableLocation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenMoveFinished, Invoker, InteractableLocation, InteractableRotation, 
			FOnInteractFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenInteractFinished, Invoker, OnInteractFinished)), 1.f);
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
			FBehaviorWithPosition Behavior = GetBehavior(Invoker, Location);
			if (Behavior.Behavior)
			{
				if (Behavior.bAttachToRotation && Invoker->CharacterTurnAction)
				{
					FTransform Transfrom = GetOwner()->GetActorTransform();
					if (Behavior.bAttachToLocation)
					{
						UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Invoker, Transfrom.TransformPosition(Behavior.Location), 1.f);
					}
					Invoker->TurnTo(Transfrom.TransformRotation(Behavior.Rotation.Quaternion()).Rotator(), FOnCharacterActionFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenTurnFinished, Invoker, Behavior, OnInteractFinished));
				}
				else
				{
					WhenTurnFinished(true, Invoker, Behavior, OnInteractFinished);
				}
				return;
			}
		}
	}

	OnInteractFinished.ExecuteIfBound(false);
}

void UARPG_InteractableActorManagerBase::WhenTurnFinished(bool Succeed, ACharacterBase* Invoker, FBehaviorWithPosition Behavior, FOnInteractFinished OnInteractFinished)
{
	if (Succeed)
	{
		Behavior.RelativePositionExecuteBehavior(Invoker, OnInteractFinished, GetOwner()->GetActorTransform());
		CurBehaviorMap.FindOrAdd(Invoker) = Behavior.Behavior;
		WhenBeginInteract(Invoker);
		OnBeginInteract.Broadcast(GetOwner(), this, Invoker);
	}
}

void UARPG_InteractableActorManagerBase::WhenInteractFinished(bool Succeed, ACharacterBase* Invoker, FOnInteractFinished OnInteractFinished)
{
	WhenEndInteract(Invoker);
	OnInteractFinished.ExecuteIfBound(Succeed);
}

void UARPG_InteractableActorManagerBase::EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished)
{
	if (UARPG_CharacterBehaviorConfigBase** P_Behavior = CurBehaviorMap.Find(Invoker))
	{
		(*P_Behavior)->AbortBehavior(Invoker, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenBehaviorAbortFinished, Invoker, OnInteractAbortFinished));
	}
	else
	{
		OnInteractAbortFinished.ExecuteIfBound();
	}
}

void UARPG_InteractableActorManagerBase::WhenBehaviorAbortFinished(ACharacterBase* Invoker, FOnInteractAbortFinished OnInteractAbortFinished)
{
	CurBehaviorMap.Remove(Invoker);
	WhenEndInteract(Invoker);
	OnEndInteract.Broadcast(GetOwner(), this, Invoker);
	OnInteractAbortFinished.ExecuteIfBound();
}

void UARPG_InteractableActorManagerBase::GetInteractableLocationAndRotation(ACharacterBase* Invoker, FVector& InteractableLocation, FRotator& InteractableRotation) const
{
	InteractableLocation = GetOwner()->GetActorLocation();
	InteractableRotation = GetOwner()->GetActorRotation();
}

UInteractableActorManagerSingle::UInteractableActorManagerSingle()
{
	Behaviors.Add(FBehaviorWithPosition());
}

FBehaviorWithPosition UInteractableActorManagerSingle::GetBehavior(ACharacterBase* Invoker, const FVector& InteractableLocation) const
{
	const FBehaviorWithPosition* Behavior = Behaviors.FindByPredicate([&](const FBehaviorWithPosition& Behavior) {return GetOwner()->GetActorTransform().TransformPosition(Behavior.Location).Equals(InteractableLocation); });
	return Behavior ? *Behavior : FBehaviorWithPosition();
}

void UInteractableActorManagerSingle::WhenBeginInteract(ACharacterBase* Invoker)
{
	Invoker->InteractableActorManager = this;
	User = Invoker;
}

void UInteractableActorManagerSingle::WhenEndInteract(ACharacterBase* Invoker)
{
	if (Invoker->InteractableActorManager == this)
	{
		Invoker->InteractableActorManager = nullptr;
		User = nullptr;
	}
}

void UInteractableActorManagerSingle::GetInteractableLocationAndRotation(ACharacterBase* Invoker, FVector& InteractableLocation, FRotator& InteractableRotation) const
{
	if (Behaviors.Num() > 0)
	{
		FTransform Transform = GetOwner()->GetActorTransform();
		const FBehaviorWithPosition* NearestBehavior = &Behaviors[0];
		float Distance = (Invoker->GetActorLocation() - Transform.TransformPosition(NearestBehavior->Location)).Size();
		for (int i = 1; i < Behaviors.Num(); ++i)
		{
			float CompareDistance = (Invoker->GetActorLocation() - Transform.TransformPosition(Behaviors[i].Location)).Size();
			if (Distance > CompareDistance)
			{
				CompareDistance = Distance;
				NearestBehavior = &Behaviors[i];
			}
		}
		InteractableLocation = GetOwner()->GetActorTransform().TransformPosition(NearestBehavior->Location);
		InteractableRotation = GetOwner()->GetActorTransform().TransformRotation(NearestBehavior->Rotation.Quaternion()).Rotator();
	}
}
