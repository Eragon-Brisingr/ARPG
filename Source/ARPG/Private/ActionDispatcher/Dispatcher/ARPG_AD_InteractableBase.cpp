// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AD_InteractableBase.h"
#include "CharacterBase.h"

void UARPG_AD_InteractableBase::InitInteractDispatcher(AActor* InInteractTarget)
{
	check(InteractTarget == nullptr);

	InteractTarget = InInteractTarget;
	InitLeader(InInteractTarget);
}

void UARPG_AD_InteractableBase::StartInteractDispatcher(ACharacterBase* InInteractInvoker, const FOnDispatchDeactiveNative& OnDispatchDeactive)
{
	check(OnDispatcherAbortedNative.IsBound() == false);

	InteractInvoker = InInteractInvoker;
	OnDispatchDeactiveNative = OnDispatchDeactive;

	CurrentActions.Empty();

	StartDispatch();
}

void UARPG_AD_InteractableBase::AbortInteractDispatcher(const FOnDispatcherAbortedNative& DispatcherAbortedEvent)
{
	check(State != EActionDispatcherState::Deactive);

	if (State == EActionDispatcherState::Active)
	{
		AbortDispatch();
	}
	else if (State == EActionDispatcherState::Aborting)
	{
		OnDispatcherAbortedNative = DispatcherAbortedEvent;
	}
}

void UARPG_AD_InteractableBase::WhenDeactived(bool IsFinsihedCompleted)
{
	Super::WhenDeactived(IsFinsihedCompleted);

	InteractInvoker = nullptr;
}
