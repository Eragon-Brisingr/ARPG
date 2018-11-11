// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_LockOnTargetSystem.h"
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/World.h>
#include "XD_MacrosLibrary.h"


FARPG_LockOnTargetSystem::FARPG_LockOnTargetSystem()
	:bEnableLockOnSystem(true)
{

}

// Add default functionality here for any IARPG_LockOnTargetSystem functions that are not pure virtual.

void FARPG_LockOnTargetSystem::LockOnTargetTick(class AController* Controller, float DeltaTime)
{
	if (!bEnableLockOnSystem || !LockedTarget.IsValid())
	{
		return;
	}

	bool TryLockAgain;
	if (!CallUnrealInterface(IARPG_LockOnTargetInterface, CanLockingOnTarget)(LockedTarget.Get(), Controller, LockedSocketName, TryLockAgain))
	{
		if (TryLockAgain)
		{
			if (TraceCanLockedActor(Controller, LockedTarget->GetActorLocation(), LockedTarget->GetActorLocation() + FVector(0.f, 0.f, 100.f), { Controller->GetPawn() }, 5000.f))
			{
				return;
			}
		}
		ClearLockedTarget();

		return;
	}

	if (ACharacter* Character = Controller->GetCharacter())
	{
		if (Character->bUseControllerRotationYaw)
		{
			return;
		}

		FVector CurLocation = Character->GetActorLocation();
		FVector TargetLocation = CallUnrealInterface(IARPG_LockOnTargetInterface, GetTargetLocation)(LockedTarget.Get(), LockedSocketName);

		float Distance = (CurLocation - TargetLocation).Size();
		//摄像机角度与锁定距离的关系
		TargetLocation.Z -= Distance / 4.f + 100.f;


		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(CurLocation, TargetLocation);
		float InterpSpeed = UKismetMathLibrary::MapRangeClamped(Distance, 0.f, 1000.f, 5.f, 0.5f);

		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), TargetRotation, DeltaTime, InterpSpeed));
	}
}

bool FARPG_LockOnTargetSystem::CanLockedOn(class AController* Controller, AActor* Target, const FName& SocketName)
{
	return Target->Implements<UARPG_LockOnTargetInterface>() && CallUnrealInterface(IARPG_LockOnTargetInterface, CanLockedOnTarget)(Target, Controller, SocketName);
}

void FARPG_LockOnTargetSystem::SetLockedTarget(AActor* Target, const FName& SocketName)
{
	LockedTarget = Target;
	LockedSocketName = SocketName;
}

void FARPG_LockOnTargetSystem::ClearLockedTarget()
{
	LockedTarget = nullptr; 
	LockedSocketName = NAME_None;
}

bool FARPG_LockOnTargetSystem::TraceCanLockedActor(class AController* Controller, const FVector& Start, const FVector& End, const TArray<AActor*>& IgnoreActors, float Radius /*= 500.f*/)
{
	TArray<FHitResult> HitResults;
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(Controller, Start, End, 500.f, TraceObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, false))
	{
		return FilterResultToLock(HitResults, Controller, IgnoreActors);
	}
	return false;
}

bool FARPG_LockOnTargetSystem::FilterResultToLock(TArray<FHitResult> HitResults, class AController* Controller, const TArray<AActor*>& IgnoreActors)
{
	for (const FHitResult& Hit : HitResults)
	{
		if (CanLockedOn(Controller, Hit.GetActor(), Hit.BoneName))
		{
			FVector TargetLocation = CallUnrealInterface(IARPG_LockOnTargetInterface, GetTargetLocation)(Hit.GetActor(), Hit.BoneName);

			FHitResult VisibleCheck;
			if (UKismetSystemLibrary::LineTraceSingle(Controller, Controller->GetPawn()->GetActorLocation(), TargetLocation, VisibilityChannel, false, IgnoreActors, EDrawDebugTrace::None, VisibleCheck, false))
			{
				if (VisibleCheck.GetActor() == Hit.GetActor())
				{
					SetLockedTarget(Hit.GetActor(), Hit.BoneName);
					return true;
				}
			}
		}
	}
	return false;
}

void FARPG_LockOnTargetSystem::ToggleLockedTarget(class AController* Controller)
{
	if (LockedTarget.IsValid())
	{
		ClearLockedTarget();
	}
	else
	{
		FVector StartLocation = Controller->GetPawn()->GetActorLocation();
		FVector EndLocation = Controller->GetControlRotation().RotateVector(FVector(MaxTraceDistance, 0.f, 0.f)) + StartLocation;
		TraceCanLockedActor(Controller, StartLocation, EndLocation, { Controller->GetPawn() });
	}
}

bool FARPG_LockOnTargetSystem::InvokeSwitchLockedTarget(class AController* Controller, const FVector& SwitchDirection)
{
	float CurrentTimeSeconds = Controller->GetWorld()->GetRealTimeSeconds();
	if (PreInvokeSwitchSeconds + 0.2f < CurrentTimeSeconds)
	{
		PreInvokeSwitchSeconds = CurrentTimeSeconds;
		if (LockedTarget.IsValid())
		{
			FVector StartLocation = LockedTarget->GetActorLocation();
			FVector EndLocation = StartLocation + SwitchDirection * MaxTraceDistance;
			return TraceCanLockedActor(Controller, StartLocation, EndLocation, { Controller->GetPawn() , LockedTarget.Get() });
		}
	}
	return false;
}
