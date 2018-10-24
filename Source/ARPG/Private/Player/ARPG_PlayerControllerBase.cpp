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

AActor* AARPG_PlayerControllerBase::GetLockedTarget() const
{
	return LockOnTargetSystem.LockedTarget.Get();
}

bool AARPG_PlayerControllerBase::SetLockedTarget(AActor* Target, const FName& SocketName)
{
	if (LockOnTargetSystem.CanLockedOn(this, Target, SocketName))
	{
		LockOnTargetSystem.SetLockedTarget(Target, SocketName);
		SetLockedTarget_ToServer(Target, SocketName);
		return true;
	}
	return false;
}

void AARPG_PlayerControllerBase::SetLockedTarget_ToServer_Implementation(AActor* Target, const FName& SocketName)
{
	if (LockOnTargetSystem.CanLockedOn(this, Target, SocketName))
	{
		LockOnTargetSystem.SetLockedTarget(Target, SocketName);
	}
}

void AARPG_PlayerControllerBase::ClearLockedTarget()
{
	LockOnTargetSystem.ClearLockedTarget();
	ClearLockedTarget_ToServer();
}

void AARPG_PlayerControllerBase::ClearLockedTarget_ToServer_Implementation()
{
	LockOnTargetSystem.ClearLockedTarget();
}

bool AARPG_PlayerControllerBase::InvokeSwitchLockedTarget(bool Left)
{
	return LockOnTargetSystem.InvokeSwitchLockedTarget(this, FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y) * (Left ? 1.f : -1.f));
}
