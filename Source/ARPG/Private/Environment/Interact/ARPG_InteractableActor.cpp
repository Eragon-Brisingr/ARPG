// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActor.h"
#include "ARPG_InteractableActorManager.h"

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

AARPG_InteractableActorSingle::AARPG_InteractableActorSingle()
{
	InteractableActorManagerSingle = CreateDefaultSubobject<UInteractableActorManagerSingle>(GET_MEMBER_NAME_CHECKED(AARPG_InteractableActorBase, InteractableActorManager));
	InteractableActorManager = InteractableActorManagerSingle;
}

AARPG_InteractableActorMulti::AARPG_InteractableActorMulti()
{
	InteractableActorManagerMulti = CreateDefaultSubobject<UInteractableActorManagerMulti>(GET_MEMBER_NAME_CHECKED(AARPG_InteractableActorBase, InteractableActorManager));
	InteractableActorManager = InteractableActorManagerMulti;
}
