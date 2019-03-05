// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_MoveTo.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"

void UARPG_DA_MoveToBase::WhenActionActived()
{
	RegisterEntity(Mover.Get());
}

void UARPG_DA_MoveToBase::WhenActionDeactived()
{
	ACharacterBase* Character = Cast<ACharacterBase>(Mover.Get());
	Character->StopMovement();
}

void UARPG_DA_MoveToBase::WhenActionFinished()
{
	UnregisterEntity(Mover.Get());
}

void UARPG_DA_MoveToBase::WhenRequestFinished(const FPathFollowingResult& Result)
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

void UARPG_DA_MoveToActor::WhenActionActived()
{
	Super::WhenActionActived();
	ACharacterBase* Character = Cast<ACharacterBase>(Mover.Get());
	UARPG_MoveUtils::ARPG_MoveToActor(Character, Goal.Get(), FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_MoveToActor::WhenRequestFinished));
}

void UARPG_DA_MoveToLocation::WhenActionActived()
{
	Super::WhenActionActived();
	ACharacterBase* Character = Cast<ACharacterBase>(Mover.Get());
	UARPG_MoveUtils::ARPG_MoveToLocation(Character, Destination, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_MoveToLocation::WhenRequestFinished));
}
