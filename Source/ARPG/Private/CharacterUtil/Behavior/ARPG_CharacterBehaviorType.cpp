// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorType.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "Interact/ARPG_InteractAnimInterface.h"
#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

UARPG_CharacterBehaviorConfigurable* FBehaviorWithPosition::WorldPositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished) const
{
	if (bAttachToLocation && bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToFitGround(Character, Location, Rotation);
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Character, Location);
	}
	else if (bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToRotationFitGround(Character, Rotation);
	}

	if (bEnableHandIK)
	{
		IARPG_InteractAnimInterface::SetEnableInteractHandIK(Character, true);
		IARPG_InteractAnimInterface::SetInteractHandIK_Location(Character, Location);
	}

	return ExecuteBehavior(Character, OnBehaviorFinished, Location);
}

UARPG_CharacterBehaviorConfigurable* FBehaviorWithPosition::RelativePositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FTransform& Transform) const
{
	FVector WorldLocation = Transform.TransformPosition(Location);
	if (bAttachToLocation && bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToFitGround(Character, WorldLocation, Transform.TransformRotation(Rotation.Quaternion()).Rotator());
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Character, WorldLocation);
	}
	else if (bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToRotationFitGround(Character, Transform.TransformRotation(Rotation.Quaternion()).Rotator());
	}

	return ExecuteBehavior(Character, OnBehaviorFinished, WorldLocation);
}

void FBehaviorWithPosition::WhenBehaviorFinished(bool Succeed, class ACharacterBase* Character, FOnCharacterBehaviorFinished OnBehaviorFinished)
{
	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->Implements<UARPG_InteractAnimInterface>())
		{
			IARPG_InteractAnimInterface::SetEnableInteractHandIK(AnimInstance, false);
		}
	}

	OnBehaviorFinished.ExecuteIfBound(Succeed);
}

UARPG_CharacterBehaviorConfigurable* FBehaviorWithPosition::ExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FVector& WorldLocation) const
{
	if (bEnableHandIK)
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			if (AnimInstance->Implements<UARPG_InteractAnimInterface>())
			{
				IARPG_InteractAnimInterface::SetEnableInteractHandIK(AnimInstance, true);
				FVector HandIKLocation = WorldLocation;
				HandIKLocation.Z = Character->GetActorLocation().Z - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				IARPG_InteractAnimInterface::SetInteractHandIK_Location(AnimInstance, HandIKLocation);
				return Behavior->ExecuteBehavior(Character, FOnCharacterBehaviorFinished::CreateStatic(&FBehaviorWithPosition::WhenBehaviorFinished, Character, OnBehaviorFinished));
			}
		}
	}

	return Behavior->ExecuteBehavior(Character, OnBehaviorFinished);
}
