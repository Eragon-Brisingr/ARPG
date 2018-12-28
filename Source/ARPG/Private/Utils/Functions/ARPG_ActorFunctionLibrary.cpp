// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ActorFunctionLibrary.h"
#include <Components/SceneComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Ticker.h>
#include <GameFramework/Actor.h>
#include "CharacterBase.h"

TMap<TWeakObjectPtr<USceneComponent>, FDelegateHandle> UARPG_ActorFunctionLibrary::MovingComponentMap;

void UARPG_ActorFunctionLibrary::MoveComponentTo(USceneComponent* Component, const FVector& TargetRelativeLocation, const FRotator& TargetRelativeRotation, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
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

void UARPG_ActorFunctionLibrary::MoveComponentToLocation(USceneComponent* Component, const FVector& TargetRelativeLocation, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
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
		MovingComponentMap.FindOrAdd(Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
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
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
	}
}

void UARPG_ActorFunctionLibrary::MoveComponentToRotation(USceneComponent* Component, const FRotator& TargetRelativeRotation, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Component)
	{
		TWeakObjectPtr<USceneComponent> _Component = Component;
		if (FDelegateHandle* TickerHandle = MovingComponentMap.Find(_Component))
		{
			FTicker::GetCoreTicker().RemoveTicker(*TickerHandle);
		}

		float _ExecuteTime = 0.f;
		const FRotator _StartRelativeRotation = Component->RelativeRotation;
		MovingComponentMap.FindOrAdd(Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
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
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
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

void UARPG_ActorFunctionLibrary::MoveActorTo(AActor* Actor, const FVector& Location, const FRotator& Rotation, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Actor)
	{
		USceneComponent* Component = Actor->GetRootComponent();
		TWeakObjectPtr<USceneComponent> _Component = Component;
		if (FDelegateHandle* TickerHandle = MovingComponentMap.Find(_Component))
		{
			FTicker::GetCoreTicker().RemoveTicker(*TickerHandle);
		}

		float _ExecuteTime = 0.f;
		const FVector _StartWorldLocation = Component->GetComponentLocation();
		const FRotator _StartWorldRotation = Component->GetComponentRotation();
		MovingComponentMap.FindOrAdd(Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FVector DeltaRelativeLocation = UKismetMathLibrary::VLerp(_StartWorldLocation, Location, _ExecuteTime / OverTime);
					const FRotator DeltaRelativeRotation = UKismetMathLibrary::RLerp(_StartWorldRotation, Rotation, _ExecuteTime / OverTime, true);
					_Component->SetWorldLocationAndRotation(DeltaRelativeLocation, DeltaRelativeRotation, Sweep);
					return true;
				}
				else
				{
					_Component->SetWorldLocationAndRotation(Location, Rotation, Sweep);
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
	}
}

void UARPG_ActorFunctionLibrary::MoveActorToLocation(AActor* Actor, const FVector& Location, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Actor)
	{
		USceneComponent* Component = Actor->GetRootComponent();
		TWeakObjectPtr<USceneComponent> _Component = Component;
		if (FDelegateHandle* TickerHandle = MovingComponentMap.Find(_Component))
		{
			FTicker::GetCoreTicker().RemoveTicker(*TickerHandle);
		}

		float _ExecuteTime = 0.f;
		const FVector _StartRelativeLocation = Component->GetComponentLocation();
		MovingComponentMap.FindOrAdd(Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FVector DeltaRelativeLocation = UKismetMathLibrary::VLerp(_StartRelativeLocation, Location, _ExecuteTime / OverTime);
					_Component->SetWorldLocation(DeltaRelativeLocation, Sweep);
					return true;
				}
				else
				{
					_Component->SetWorldLocation(Location, Sweep);
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
	}
}

void UARPG_ActorFunctionLibrary::MoveActorToRotation(AActor* Actor, const FRotator& Rotation, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Actor)
	{
		USceneComponent* Component = Actor->GetRootComponent();
		TWeakObjectPtr<USceneComponent> _Component = Component;
		if (FDelegateHandle* TickerHandle = MovingComponentMap.Find(_Component))
		{
			FTicker::GetCoreTicker().RemoveTicker(*TickerHandle);
		}

		float _ExecuteTime = 0.f;
		const FRotator _StartWorldRotation = Component->GetComponentRotation();
		MovingComponentMap.FindOrAdd(Component) = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([=](float DeltaSeconds) mutable
		{
			if (_Component.IsValid())
			{
				if (_ExecuteTime < OverTime)
				{
					_ExecuteTime += DeltaSeconds;
					const FRotator DeltaRelativeRotation = UKismetMathLibrary::RLerp(_StartWorldRotation, Rotation, _ExecuteTime / OverTime, true);
					_Component->SetWorldRotation(DeltaRelativeRotation, Sweep);
					return true;
				}
				else
				{
					_Component->SetWorldRotation(Rotation, Sweep);
				}
			}
			MovingComponentMap.Remove(_Component);
			return false;
		}));
	}
}

void UARPG_ActorFunctionLibrary::PushActorTo(AActor* Actor, const FVector& Distance, float OverTime /*= 0.2f*/, bool Sweep /*= true*/)
{
	if (Actor)
	{
		PushComponentTo(Actor->GetRootComponent(), Distance, OverTime, Sweep);
	}
}

void UARPG_ActorFunctionLibrary::MoveCharacterToFitGround(ACharacterBase* Character, const FVector& Location, const FRotator& Rotator, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Character)
	{
		FVector MoveLocation = Location;
		MoveLocation.Z = Character->GetActorLocation().Z;
		MoveActorTo(Character, MoveLocation, Rotator, OverTime, Sweep);
	}
}

void UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(ACharacterBase* Character, const FVector& Location, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Character)
	{
		FVector MoveLocation = Location;
		MoveLocation.Z = Character->GetActorLocation().Z;
		MoveActorToLocation(Character, MoveLocation, OverTime, Sweep);
	}
}

void UARPG_ActorFunctionLibrary::MoveCharacterToRotationFitGround(ACharacterBase* Character, const FRotator& Rotator, float OverTime /*= 0.2f*/, bool Sweep /*= false*/)
{
	if (Character)
	{
		MoveActorToRotation(Character, Rotator, OverTime, Sweep);
	}
}
