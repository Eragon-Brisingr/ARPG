// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AD_InteractableBase.h"
#include "CharacterBase.h"

void UARPG_AD_InteractableBase::StartInteractDispatcher(AActor* InInteractTarget, ACharacterBase* InInteractInvoker, const FWhenDispatchFinishedNative& DispatchFinishedEvent, const FOnActionDispatcherAbortedNative::FDelegate& DispatcherAbortedEvent)
{
	if (DispatcherLeader == nullptr)
	{
		InitLeader(InInteractTarget);
	}

	InteractTarget = InInteractTarget;
	InteractInvoker = InInteractInvoker;
	WhenDispatchFinishedNative = DispatchFinishedEvent;

	OnActionDispatcherAbortedNative.Clear();
	OnActionDispatcherAbortedNative.Add(DispatcherAbortedEvent);

	CurrentActions.Empty();

	StartDispatch();
}

void UARPG_AD_InteractableBase::AbortInteractDispatcher(const FOnActionDispatcherAbortedNative::FDelegate& DispatcherAbortedEvent)
{
	OnActionDispatcherAbortedNative.Add(DispatcherAbortedEvent);
	AbortDispatch({});
}
