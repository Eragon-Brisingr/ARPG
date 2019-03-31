// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ActorFunctionLibrary.h"
#include <Components/SceneComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Ticker.h>
#include <GameFramework/Actor.h>
#include "CharacterBase.h"

TMap<TWeakObjectPtr<USceneComponent>, UARPG_ActorMoveUtils::FActorMoveData> UARPG_ActorMoveUtils::MovingComponentMap;

void UARPG_ActorMoveUtils::MoveComponentTo(USceneComponent* Component, const FVector& TargetRelativeLocation, const FRotator& TargetRelativeRotation, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Component)
	{
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		const FVector _StartRelativeLocation = Component->RelativeLocation;
		const FRotator _StartRelativeRotation = Component->RelativeRotation;

		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
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
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::MoveComponentToLocation(USceneComponent* Component, const FVector& TargetRelativeLocation, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Component)
	{
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		const FVector _StartRelativeLocation = Component->RelativeLocation;
		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FVector DeltaRelativeLocation = UKismetMathLibrary::VLerp(_StartRelativeLocation, TargetRelativeLocation, _ExecuteTime / OverTime);
					_Component->SetRelativeLocation(DeltaRelativeLocation, Sweep);
					return true;
				}
				else
				{
					_Component->SetRelativeLocation(TargetRelativeLocation, Sweep);
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::MoveComponentToRotation(USceneComponent* Component, const FRotator& TargetRelativeRotation, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Component)
	{
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		const FRotator _StartRelativeRotation = Component->RelativeRotation;
		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FRotator DeltaRelativeRotation = UKismetMathLibrary::RLerp(_StartRelativeRotation, TargetRelativeRotation, _ExecuteTime / OverTime, true);
					_Component->SetRelativeRotation(DeltaRelativeRotation, Sweep);
					return true;
				}
				else
				{
					_Component->SetRelativeRotation(TargetRelativeRotation, Sweep);
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::PushComponentTo(USceneComponent* Component, const FVector& Distance, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Component)
	{
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;

					_Component->AddWorldOffset(Distance * DeltaSeconds / OverTime, Sweep);
					return true;
				}
				else
				{
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(_Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::MoveActorTo(AActor* Actor, const FVector& Location, const FRotator& Rotation, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Actor)
	{
		USceneComponent* Component = Actor->GetRootComponent();
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		const FVector _StartWorldLocation = Component->GetComponentLocation();
		const FRotator _StartWorldRotation = Component->GetComponentRotation();
		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FVector DeltaWorldLocation = UKismetMathLibrary::VLerp(_StartWorldLocation, Location, _ExecuteTime / OverTime);
					const FRotator DeltaWorldRotation = UKismetMathLibrary::RLerp(_StartWorldRotation, Rotation, _ExecuteTime / OverTime, true);
					_Component->SetWorldLocationAndRotation(DeltaWorldLocation, DeltaWorldRotation, Sweep);
					return true;
				}
				else
				{
					_Component->SetWorldLocationAndRotation(Location, Rotation, Sweep);
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(_Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::MoveActorToLocation(AActor* Actor, const FVector& Location, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Actor)
	{
		USceneComponent* Component = Actor->GetRootComponent();
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		const FVector _StartRelativeLocation = Component->GetComponentLocation();
		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FVector DeltaWorldLocation = UKismetMathLibrary::VLerp(_StartRelativeLocation, Location, _ExecuteTime / OverTime);
					_Component->SetWorldLocation(DeltaWorldLocation, Sweep);
					return true;
				}
				else
				{
					_Component->SetWorldLocation(Location, Sweep);
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::MoveActorToRotation(AActor* Actor, const FRotator& Rotation, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Actor)
	{
		USceneComponent* Component = Actor->GetRootComponent();
		TWeakObjectPtr<USceneComponent> _Component = Component;
		StopComponentMove(Component);

		float _ExecuteTime = 0.f;
		const FRotator _StartWorldRotation = Component->GetComponentRotation();
		FDelegateHandle TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FRotator DeltaWorldRotation = UKismetMathLibrary::RLerp(_StartWorldRotation, Rotation, _ExecuteTime / OverTime, true);
					_Component->SetWorldRotation(DeltaWorldRotation, Sweep);
					return true;
				}
				else
				{
					_Component->SetWorldRotation(Rotation, Sweep);
					MovingComponentMap.FindAndRemoveChecked(_Component).OnActorMoveFinished.ExecuteIfBound(false);
					return false;
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
		MovingComponentMap.FindOrAdd(Component) = FActorMoveData(TickerHandle, OnMoveFinished);
	}
}

void UARPG_ActorMoveUtils::PushActorTo(AActor* Actor, const FVector& Distance, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Actor)
	{
		PushComponentTo(Actor->GetRootComponent(), Distance, OnMoveFinished, OverTime, Sweep);
	}
}

void UARPG_ActorMoveUtils::MoveCharacterToFitGround(ACharacterBase* Character, const FVector& Location, const FRotator& Rotator, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Character)
	{
		FVector MoveLocation = Location;
		MoveLocation.Z = Character->GetActorLocation().Z;
		MoveActorTo(Character, MoveLocation, Rotator, OnMoveFinished, OverTime, Sweep);
	}
}

void UARPG_ActorMoveUtils::MoveCharacterToLocationFitGround(ACharacterBase* Character, const FVector& Location, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Character)
	{
		FVector MoveLocation = Location;
		MoveLocation.Z = Character->GetActorLocation().Z;
		MoveActorToLocation(Character, MoveLocation, OnMoveFinished, OverTime, Sweep);
	}
}

void UARPG_ActorMoveUtils::MoveCharacterToRotationFitGround(ACharacterBase* Character, const FRotator& Rotator, const FOnActorMoveFinished& OnMoveFinished, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Character)
	{
		MoveActorToRotation(Character, Rotator, OnMoveFinished, OverTime, Sweep);
	}
}

void UARPG_ActorMoveUtils::StopComponentMove(USceneComponent* Component)
{
	if (Component)
	{
		if (FActorMoveData* ActorMoveData = MovingComponentMap.Find(Component))
		{
			FTicker::GetCoreTicker().RemoveTicker(ActorMoveData->TickerHandle);
			ActorMoveData->OnActorMoveFinished.ExecuteIfBound(true);
			MovingComponentMap.Remove(Component);
		}
	}
}

void UARPG_ActorMoveUtils::StopActorMove(AActor* Actor)
{
	if (Actor)
	{
		StopComponentMove(Actor->GetRootComponent());
	}
}
