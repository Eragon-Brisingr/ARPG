// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ActorFunctionLibrary.h"
#include <Components/SceneComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Ticker.h>
#include <GameFramework/Actor.h>

TMap<TWeakObjectPtr<USceneComponent>, FDelegateHandle> UARPG_ActorFunctionLibrary::MovingComponentMap;

void UARPG_ActorFunctionLibrary::MoveComponentTo(USceneComponent* Component, const FVector& TargetRelativeLocation, const FRotator& TargetRelativeRotation, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Component)
	{
		if (Component)
		{
			TWeakObjectPtr<USceneComponent> _Component = Component;
			if (FDelegateHandle* TickerHandle = MovingComponentMap.Find(_Component))
			{
				FTicker::GetCoreTicker().RemoveTicker(*TickerHandle);
			}

			float _ExecuteTime = 0.f;
			const FVector _StartRelativeLocation = Component->RelativeLocation;
			const FRotator _StartRelativeRotation = Component->RelativeRotation;
			MovingComponentMap.FindOrAdd(Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
			{
				if (_Component.IsValid())
				{
					if (_ExecuteTime < OverTime)
					{
						_ExecuteTime += DeltaSeconds;
						const FVector DeltaRelativeLocation = UKismetMathLibrary::VLerp(_StartRelativeLocation, TargetRelativeLocation, _ExecuteTime / OverTime);
						const FRotator DeltaRelativeRotation = UKismetMathLibrary::RLerp(_StartRelativeRotation, TargetRelativeRotation, _ExecuteTime / OverTime, true);
						_Component->SetRelativeLocationAndRotation(DeltaRelativeLocation, DeltaRelativeRotation, Sweep);
						return true;
					}
					else
					{
						_Component->SetRelativeLocationAndRotation(TargetRelativeLocation, TargetRelativeRotation, Sweep);
					}
				}
				MovingComponentMap.Remove(_Component);
				return false;
			}));
		}
	}
}

void UARPG_ActorFunctionLibrary::PushComponentTo(USceneComponent* Component, const FVector& Distance, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Component)
	{
		TWeakObjectPtr<USceneComponent> _Component = Component;
		if (FDelegateHandle* TickerHandle = MovingComponentMap.Find(_Component))
		{
			FTicker::GetCoreTicker().RemoveTicker(*TickerHandle);
		}

		float _ExecuteTime = 0.f;
		MovingComponentMap.FindOrAdd(_Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;

					_Component->AddWorldOffset(Distance * DeltaSeconds / OverTime, Sweep);
					return true;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
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
