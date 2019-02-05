// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_PlayerControllerBase.h"
#include "CharacterBase.h"
#include "ARPG_PathFollowingComponent.h"
#include "UnrealNetwork.h"
#include "ARPG_GameTaskManager.h"


AARPG_PlayerControllerBase::AARPG_PlayerControllerBase()
{
	PathFollowingComponent = CreateDefaultSubobject<UARPG_PathFollowingComponent>(GET_MEMBER_NAME_CHECKED(AARPG_PlayerControllerBase, PathFollowingComponent));

	GameTaskManager = CreateDefaultSubobject<UARPG_GameTaskManager>(GET_MEMBER_NAME_CHECKED(AARPG_PlayerControllerBase, GameTaskManager));
}

void AARPG_PlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() || IsLocalController())
	{
		LockOnTargetSystem.LockOnTargetTick(this, DeltaSeconds);
		if (ACharacterBase* ControlledCharacter = Cast<ACharacterBase>(GetPawn()))
		{
			ControlledCharacter->SetLockedTarget(LockOnTargetSystem.LockedTarget.Get());
		}
	}
}

void AARPG_PlayerControllerBase::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPG_PlayerControllerBase, bIsInPathFollowing);
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
	LockOnTargetSystem.SetLockedTarget(Target, SocketName);
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

bool AARPG_PlayerControllerBase::ToggleLockedTarget()
{
	LockOnTargetSystem.ToggleLockedTarget(this);
	if (LockOnTargetSystem.LockedTarget.IsValid())
	{
		SetLockedTarget_ToServer(LockOnTargetSystem.LockedTarget.Get(), LockOnTargetSystem.LockedSocketName);
		return true;
	}
	else
	{
		ClearLockedTarget_ToServer();
		return false;
	}
}

bool AARPG_PlayerControllerBase::InvokeSwitchLockedTarget(bool Left)
{
	bool Succeed = LockOnTargetSystem.InvokeSwitchLockedTarget(this, FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y) * (Left ? 1.f : -1.f));
	if (Succeed && LockOnTargetSystem.LockedTarget.IsValid())
	{
		SetLockedTarget_ToServer(LockOnTargetSystem.LockedTarget.Get(), LockOnTargetSystem.LockedSocketName);
	}
	return Succeed;
}

void AARPG_PlayerControllerBase::OnRep_bIsInPathFollowing()
{

}
