// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanBase.h"
#include "ARPG_MovementComponent.h"

AHumanBase::AHumanBase(const FObjectInitializer& PCIP)
	:Super(PCIP)
{
	ARPG_MovementComponent->MovementState.bCanCrouch = true;
}
