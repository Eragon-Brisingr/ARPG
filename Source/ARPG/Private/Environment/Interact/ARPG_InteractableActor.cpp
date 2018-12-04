// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActor.h"
#include "ARPG_InteractableActorManager.h"

AARPG_InteractableActorBase::AARPG_InteractableActorBase()
{
	InteractableActorManager = CreateDefaultSubobject<UARPG_InteractableActorManagerSample>(GET_MEMBER_NAME_CHECKED(AARPG_InteractableActorBase, InteractableActorManager));
}

void AARPG_InteractableActorBase::StartInteract(ACharacterBase* Invoker, const FOnInteractFinished& OnInteractFinished)
{
	InteractableActorManager->StartInteract(Invoker, OnInteractFinished);
}

void AARPG_InteractableActorBase::EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished)
{
	InteractableActorManager->EndInteract(Invoker, OnInteractAbortFinished);
}

bool AARPG_InteractableActorBase::CanInteract_Implementation(const class ACharacterBase* InteractInvoker) const
{
	return InteractableActorManager->CanInteract(InteractInvoker);
}

void AARPG_InteractableActorBase::WhenInvokeInteract_Implementation(class ACharacterBase* InteractInvoker)
{
	StartInteract(InteractInvoker, {});
}
