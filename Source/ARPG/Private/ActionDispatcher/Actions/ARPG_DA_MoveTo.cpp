// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_MoveTo.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"

void UARPG_DA_MoveTo::WhenActionActived()
{
	ACharacterBase* Mover = Cast<ACharacterBase>(Pawn.Get());
	RegisterEntity(Mover);

	if (AActor* Target = Goal.Get())
	{
		UARPG_MoveUtils::ARPG_MoveToActor(Mover, Target, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_MoveTo::WhenRequestFinished));
	}
	else
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(Mover, Location, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_MoveTo::WhenRequestFinished));
	}
}

void UARPG_DA_MoveTo::WhenActionDeactived()
{
	ACharacterBase* Mover = Cast<ACharacterBase>(Pawn.Get());
	Mover->StopMovement();
}

void UARPG_DA_MoveTo::WhenActionFinished()
{
	APawn* Mover = Pawn.Get();
	UnregisterEntity(Mover);
}

void UARPG_DA_MoveTo::WhenRequestFinished(const FPathFollowingResult& Result)
{
	if (Result.Code == EPathFollowingResult::Success)
	{
		FinishAction();
		WhenReached.ExecuteIfBound();
	}
	else if (Result.Code != EPathFollowingResult::Aborted)
	{
		FinishAction();
		WhenCanNotReached.ExecuteIfBound();
	}
}
