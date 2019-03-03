// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_PlaySequence.h"
#include "Navigation/PathFollowingComponent.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"

bool UARPG_DA_PlaySequence::MoveToSequencePlayLocation(APawn* Mover, const FVector& PlayLocation, const FRotator& PlayRotation, int32 MoverIdx)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(Mover))
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(Character, PlayLocation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_PlaySequence::WhenMoveFinished, MoverIdx));
		return true;
	}
	return Super::MoveToSequencePlayLocation(Mover, PlayLocation, PlayRotation, MoverIdx);
}

void UARPG_DA_PlaySequence::WhenMoveFinished(const FPathFollowingResult& Result, int32 MoverIdx)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		WhenMoveReached(MoverIdx);
	}
}
