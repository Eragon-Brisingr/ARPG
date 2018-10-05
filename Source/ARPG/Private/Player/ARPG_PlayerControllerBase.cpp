// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_PlayerControllerBase.h"




void AARPG_PlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalController())
	{
		LockOnTargetSystem.LockOnTargetTick(this, DeltaSeconds);
	}
}

bool AARPG_PlayerControllerBase::SetLockedTarget(AActor* Target, const FName& SocketName)
{
	if (LockOnTargetSystem.CanLockedOn(this, Target, SocketName))
	{
		LockOnTargetSystem.SetLockedTarget(Target, SocketName);
		return true;
	}
	return false;
}

bool AARPG_PlayerControllerBase::InvokeSwitchLockedTarget(bool Left)
{
	return LockOnTargetSystem.InvokeSwitchLockedTarget(this, FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y) * (Left ? 1.f : -1.f));
}
