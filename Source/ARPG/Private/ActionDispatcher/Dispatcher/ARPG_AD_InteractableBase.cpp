// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AD_InteractableBase.h"
#include "CharacterBase.h"

void UARPG_AD_InteractableBase::InitInteractDispatcher(AActor* InInteractTarget)
{
	check(InteractTarget == nullptr);

	InteractTarget = InInteractTarget;
	InitLeader(InInteractTarget);
}

void UARPG_AD_InteractableBase::StartInteractDispatcher(ACharacterBase* InInteractInvoker, const FWhenDispatchFinishedNative& DispatchFinishedEvent, const FOnActionDispatcherAbortedNative& DispatcherAbortedEvent)
{
	check(OnActionDispatcherAbortedNative.IsBound() == false);

	InteractInvoker = InInteractInvoker;
	WhenDispatchFinishedNative = DispatchFinishedEvent;

	OnActionDispatcherAbortedNative = DispatcherAbortedEvent;

	CurrentActions.Empty();

	StartDispatch();
}

void UARPG_AD_InteractableBase::AbortInteractDispatcher(const FOnActionDispatcherAbortedNative& DispatcherAbortedEvent)
{
	OnActionDispatcherAbortedNative = DispatcherAbortedEvent;
	AbortDispatch({});
}

void UARPG_AD_InteractableBase::WhenDeactived()
{
	Super::WhenDeactived();

	InteractTarget = nullptr;
}
