// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActor.h"
#include "ARPG_AD_InteractableBase.h"
#include "ARPG_MoveUtils.h"
#include "CharacterBase.h"

AARPG_InteractableActorSingle::AARPG_InteractableActorSingle()
	:StartBehaviousRadius(0.f), bSnapRotation(true)
{

}

ACharacterBase* AARPG_InteractableActorSingle::GetInteracter() const
{
	return InteractDispatcher ? InteractDispatcher->InteractInvoker.Get() : nullptr;
}

bool AARPG_InteractableActorSingle::CanInteract_Implementation(const ACharacterBase* InteractInvoker) const
{
	return InteractDispatcher && InteractDispatcher->State == EActionDispatcherState::Deactive;
}

void AARPG_InteractableActorSingle::WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker)
{
	FVector WorldLocation = GetActorTransform().TransformPosition(InteractLocation.GetLocation());

	FOnARPG_MoveFinished OnARPG_MoveFinished = FOnARPG_MoveFinished::CreateWeakLambda(this, [=](const FPathFollowingResult & Result)
		{
			if (Result.IsSuccess())
			{
				if (CanInteract(this, InteractInvoker))
				{
					InteractDispatcher->StartInteractDispatcher(InteractInvoker,
						FWhenDispatchFinishedNative::CreateWeakLambda(this, [=](const FName & Tag)
							{
								InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Succeed);
							}),
						FOnActionDispatcherAbortedNative::CreateWeakLambda(this, [=]()
							{
								InteractInvoker->ExecuteInteractEnd(EInteractEndResult::Failed);
							}));
				}
				else
				{
					InteractInvoker->ExecuteInteractEnd(EInteractEndResult::CanNotInteract);
				}
			}
			else
			{
				InteractInvoker->ExecuteInteractEnd(EInteractEndResult::CanNotInteract);
			}
		});

	if (bSnapRotation)
	{
		FRotator WorldRotation = GetActorTransform().TransformRotation(InteractLocation.GetRotation()).Rotator();
		UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(InteractInvoker, WorldLocation, WorldRotation, OnARPG_MoveFinished, StartBehaviousRadius);
	}
	else
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(InteractInvoker, WorldLocation, OnARPG_MoveFinished, StartBehaviousRadius);
	}
}

void AARPG_InteractableActorSingle::WhenAbortInteract_Implementation(ACharacterBase* InteractInvoker)
{
	if (InteractInvoker == GetInteracter())
	{
		InteractDispatcher->AbortInteractDispatcher(FOnActionDispatcherAbortedNative::CreateWeakLambda(this, [=]()
			{
				InteractInvoker->ExecuteInteractAbortEnd();
			}));
	}
	else
	{
		InteractInvoker->StopMovement();
		InteractInvoker->ExecuteInteractAbortEnd();
	}
}

void AARPG_InteractableActorSingle::BeginPlay()
{
	Super::BeginPlay();

	if (InteractDispatcher)
	{
		InteractDispatcher->InitInteractDispatcher(this);
	}
}
