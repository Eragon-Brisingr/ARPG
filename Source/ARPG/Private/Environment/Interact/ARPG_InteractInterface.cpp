// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractInterface.h"


// Add default functionality here for any IARPG_InteractInterface functions that are not pure virtual.
bool IsOverrideWhenInvokeInteract;

void IARPG_InteractInterface::WhenInvokeInteract_Implementation(class ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event)
{
	IsOverrideWhenInvokeInteract = false;
}

void IARPG_InteractInterface::WhenInvokeInteract(UObject* Obj, class ACharacterBase* InteractInvoker, const FOnInteractEndEvent& Event)
{
	IsOverrideWhenInvokeInteract = true;
	IARPG_InteractInterface::Execute_WhenInvokeInteract(Obj, InteractInvoker, Event);
	if (IsOverrideWhenInvokeInteract == false)
	{
		WhenExecuteInteract(Obj, InteractInvoker, Event);
	}
}
