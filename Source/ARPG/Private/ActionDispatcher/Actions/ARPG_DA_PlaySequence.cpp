// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_PlaySequence.h"
#include "Navigation/PathFollowingComponent.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_ActorFunctionLibrary.h"

bool UARPG_DA_PlaySequence::MoveToSequencePlayLocation(APawn* Mover, const FVector& PlayLocation, const FRotator& PlayRotation, int32 MoverIdx)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(Mover))
	{
		UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(Character, PlayLocation, PlayRotation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_PlaySequence::WhenMoveFinished, MoverIdx, Character, PlayLocation, PlayRotation));
		return true;
	}
	return Super::MoveToSequencePlayLocation(Mover, PlayLocation, PlayRotation, MoverIdx);
}

void UARPG_DA_PlaySequence::WhenActionDeactived()
{
	StopSequencePlayer();

	for (const FPlaySequenceMoveToData& Data : PlaySequenceMoveToDatas)
	{
		if (ACharacterBase* Character = Cast<ACharacterBase>(Data.PawnRef.Get()))
		{
			Character->StopMovement();
		}
	}
}

void UARPG_DA_PlaySequence::WhenMoveFinished(const FPathFollowingResult& Result, int32 MoverIdx, ACharacterBase* Character, FVector PlayLocation, FRotator PlayRotation)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		UARPG_ActorMoveUtils::MoveCharacterToFitGround(Character, PlayLocation, PlayRotation);
		WhenMoveReached(MoverIdx);
	}
	else
	{
		WhenMoveCanNotReached(MoverIdx);
	}
}
