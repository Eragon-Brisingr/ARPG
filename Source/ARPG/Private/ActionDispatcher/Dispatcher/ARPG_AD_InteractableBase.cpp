// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AD_InteractableBase.h"
#include "CharacterBase.h"

UARPG_AD_InteractableBase::UARPG_AD_InteractableBase()
{
	bIsMainDispatcher = false;
}

void UARPG_AD_InteractableBase::InitInteractDispatcher(AActor* InInteractTarget)
{
	check(InteractTarget == nullptr);

	InteractTarget = InInteractTarget;
	InitLeader(InInteractTarget);
}

void UARPG_AD_InteractableBase::StartInteractDispatcher(ACharacterBase* InInteractInvoker, const FOnDispatchDeactiveNative& OnDispatchDeactive)
{
	InteractInvoker = InInteractInvoker;
	OnDispatchDeactiveNative = OnDispatchDeactive;

	CurrentActions.Empty();

	StartDispatch();
}

void UARPG_AD_InteractableBase::AbortInteractDispatcher(const FOnDispatcherAbortedNative& DispatcherAbortedEvent)
{
	check(State != EActionDispatcherState::Deactive);

	OnDispatcherAbortedNative = DispatcherAbortedEvent;
	if (State == EActionDispatcherState::Active)
	{
		AbortDispatch();
	}
}

void UARPG_AD_InteractableBase::WhenDeactived(bool IsFinsihedCompleted)
{
	Super::WhenDeactived(IsFinsihedCompleted);

	InteractInvoker = nullptr;
}
