// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ActorFunctionLibrary.h"
#include <Components/SceneComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Ticker.h>
#include <GameFramework/Actor.h>

void UARPG_ActorFunctionLibrary::MoveComponentTo(USceneComponent* Component, const FVector& TargetRelativeLocation, const FRotator& TargetRelativeRotation, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Component)
	{
		class FMoveComponentHelper
		{
		public:
			TWeakObjectPtr<USceneComponent> _Component;
			float _OverTime;
			float _ExecuteTime = 0.f;
			FVector _StartRelativeLocation;
			FVector _TargetRelativeLocation;
			FRotator _StartRelativeRotation;
			FRotator _TargetRelativeRotation;
			bool _bSweep;

			bool Tick(float DeltaSeconds)
			{
				if (_Component.IsValid())
				{
					if (_ExecuteTime < _OverTime)
					{
						_ExecuteTime += DeltaSeconds;
						FVector DeltaRelativeLocation = UKismetMathLibrary::VLerp(_StartRelativeLocation, _TargetRelativeLocation, _ExecuteTime / _OverTime);
						FRotator DeltaRelativeRotation = UKismetMathLibrary::RLerp(_StartRelativeRotation, _TargetRelativeRotation, _ExecuteTime / _OverTime, true);
						_Component->SetRelativeLocationAndRotation(DeltaRelativeLocation, DeltaRelativeRotation, _bSweep);
						return true;
					}
					else
					{
						_Component->SetRelativeLocationAndRotation(_TargetRelativeLocation, _TargetRelativeRotation, _bSweep);
					}
				}
				delete this;
				return false;
			}
		};

		if (Component)
		{
			FMoveComponentHelper* MoveComponentHelper(new FMoveComponentHelper());
			MoveComponentHelper->_Component = Component;
			MoveComponentHelper->_StartRelativeLocation = Component->RelativeLocation;
			MoveComponentHelper->_TargetRelativeLocation = TargetRelativeLocation;
			MoveComponentHelper->_StartRelativeRotation = Component->RelativeRotation;
			MoveComponentHelper->_TargetRelativeRotation = TargetRelativeRotation;
			MoveComponentHelper->_OverTime = OverTime;
			MoveComponentHelper->_bSweep = Sweep;
			FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(MoveComponentHelper, &FMoveComponentHelper::Tick));
		}
	}
}

void UARPG_ActorFunctionLibrary::PushComponentTo(USceneComponent* Component, const FVector& Distance, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Component)
	{
		class FPushComponentHelper
		{
		public:
			TWeakObjectPtr<USceneComponent> _Component;
			float _OverTime;
			float _ExecuteTime = 0.f;
			FVector _Distance;
			bool _bSweep;

			bool Tick(float DeltaSeconds)
			{
				if (_Component.IsValid())
				{
					if (_ExecuteTime < _OverTime)
					{
						_ExecuteTime += DeltaSeconds;

						_Component->AddWorldOffset(_Distance * DeltaSeconds / _OverTime, _bSweep);
						return true;
					}
				}
				delete this;
				return false;
			}
		};

		if (Component)
		{
			FPushComponentHelper* MoveComponentHelper(new FPushComponentHelper());
			MoveComponentHelper->_Component = Component;
			MoveComponentHelper->_Distance = Distance;
			MoveComponentHelper->_OverTime = OverTime;
			MoveComponentHelper->_bSweep = Sweep;
			FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(MoveComponentHelper, &FPushComponentHelper::Tick));
		}
	}
}

void UARPG_ActorFunctionLibrary::MoveActorTo(AActor* Actor, const FVector& Location, const FRotator& Rotator, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Actor)
	{
		MoveComponentTo(Actor->GetRootComponent(), Location, Rotator, OverTime, Sweep);
	}
}

void UARPG_ActorFunctionLibrary::PushActorTo(AActor* Actor, const FVector& Distance, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Actor)
	{
		PushComponentTo(Actor->GetRootComponent(), Distance, OverTime, Sweep);
	}
}
