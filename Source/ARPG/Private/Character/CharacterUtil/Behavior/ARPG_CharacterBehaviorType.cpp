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
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocationAndRotation(Location, Rotation);
		}
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Character, Location);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocation(Location);
		}
	}
	else if (bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToRotationFitGround(Character, Rotation);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldRotation(Rotation);
		}
	}

	return ExecuteBehavior(Character, OnBehaviorFinished, Location);
}

UARPG_CharacterBehaviorConfigurable* FBehaviorWithPosition::RelativePositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, AActor* Owner) const
{
	const FTransform Transform = Owner->GetActorTransform();
	FVector WorldLocation = Transform.TransformPosition(Location);
	if (bAttachToLocation && bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToFitGround(Character, WorldLocation, Transform.TransformRotation(Rotation.Quaternion()).Rotator());
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocationAndRotation(WorldLocation, Rotation);
		}
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Character, WorldLocation);
		if (Character->GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
		{
			Character->ForceSetClientWorldLocation(WorldLocation);
		}
	}
	else if (bAttachToRotation)
	{
		FRotator WorldRotation = Transform.TransformRotation(Rotation.Quaternion()).Rotator();
		UARPG_ActorFunctionLibrary::MoveCharacterToRotationFitGround(Character, WorldRotation);
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
	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->Implements<UARPG_InteractAnimInterface>())
		{
			IARPG_InteractAnimInterface::SetEnableInteractHandIK(AnimInstance, false);
		}
	}

	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

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
