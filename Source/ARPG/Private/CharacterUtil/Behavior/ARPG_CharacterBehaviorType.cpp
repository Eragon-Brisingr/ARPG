// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorType.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_CharacterBehaviorBase.h"

void FBehaviorWithPosition::WorldPositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished) const
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

	Behavior->ExecuteBehavior(Character, OnBehaviorFinished);
}

void FBehaviorWithPosition::RelativePositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FTransform& Transform) const
{
	if (bAttachToLocation && bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToFitGround(Character, Transform.TransformPosition(Location), Transform.TransformRotation(Rotation.Quaternion()).Rotator());
	}
	else if (bAttachToLocation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Character, Transform.TransformPosition(Location));
	}
	else if (bAttachToRotation)
	{
		UARPG_ActorFunctionLibrary::MoveCharacterToRotationFitGround(Character, Transform.TransformRotation(Rotation.Quaternion()).Rotator());
	}

	Behavior->ExecuteBehavior(Character, OnBehaviorFinished);
}
