// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorType.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "Interact/ARPG_InteractAnimInterface.h"
#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

UARPG_CharacterBehaviorBase* FBehaviorWithPosition::WorldPositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished) const
{
	if (bAttachToLocation && bAttachToRotation)
	{
		UARPG_ActorMoveUtils::MoveCharacterToFitGround(Character, Location, Rotation);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocationAndRotation(Location, Rotation);
		}
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorMoveUtils::MoveCharacterToLocationFitGround(Character, Location);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocation(Location);
		}
	}
	else if (bAttachToRotation)
	{
		UARPG_ActorMoveUtils::MoveCharacterToRotationFitGround(Character, Rotation);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldRotation(Rotation);
		}
	}

	return ExecuteBehavior(Character, OnBehaviorFinished, Location);
}

UARPG_CharacterBehaviorBase* FBehaviorWithPosition::RelativePositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, AActor* Owner) const
{
	const FTransform Transform = Owner->GetActorTransform();
	FVector WorldLocation = Transform.TransformPosition(Location);
	if (bAttachToLocation && bAttachToRotation)
	{
		UARPG_ActorMoveUtils::MoveCharacterToFitGround(Character, WorldLocation, Transform.TransformRotation(Rotation.Quaternion()).Rotator());
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocationAndRotation(WorldLocation, Rotation);
		}
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorMoveUtils::MoveCharacterToLocationFitGround(Character, WorldLocation);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocation(WorldLocation);
		}
	}
	else if (bAttachToRotation)
	{
		FRotator WorldRotation = Transform.TransformRotation(Rotation.Quaternion()).Rotator();
		UARPG_ActorMoveUtils::MoveCharacterToRotationFitGround(Character, WorldRotation);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldRotation(WorldRotation);
		}
	}

	if (bAttachToActor)
	{
		Character->AttachToActor(Owner, FAttachmentTransformRules::KeepWorldTransform);
	}

	return ExecuteBehavior(Character, OnBehaviorFinished, WorldLocation);
}

void FBehaviorWithPosition::WhenBehaviorFinished(bool Succeed, class ACharacterBase* Character, FOnCharacterBehaviorFinished OnBehaviorFinished)
{
	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	OnBehaviorFinished.ExecuteIfBound(Succeed);
}

UARPG_CharacterBehaviorBase* FBehaviorWithPosition::ExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FVector& WorldLocation) const
{
	Behavior->ExecuteBehavior(Character, OnBehaviorFinished);
	return Behavior;
}
