// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_MoveTo.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "NavigationSystem.h"

bool UARPG_DA_MoveToBase::IsActionValid() const
{
	APawn* Pawn = Mover.Get();
	return Pawn != nullptr && Pawn->GetController() != nullptr;
}

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
		ExecuteEventAndFinishAction(WhenReached);
	}
	else if (Result.Code != EPathFollowingResult::Aborted)
	{
		ExecuteEventAndFinishAction(WhenCanNotReached);
	}
	else
	{
		AbortDispatcher();
	}
}

bool UARPG_DA_MoveToBase::IsExistValidPath(const FVector& Start, const FVector& End) const
{
	UNavigationSystemV1* const NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)
	{
		return false;
	}
	const ANavigationData* NavData = NavSys->GetNavDataForProps(FNavAgentProperties::DefaultProperties);
	if (!NavData)
	{
		return false;
	}
	FPathFindingQuery Query(this, *NavData, Start, End);
	return NavSys->TestPathSync(Query);
}

bool UARPG_DA_MoveToActor::IsActionValid() const
{
	return Super::IsActionValid() && Goal.Get() != nullptr && IsExistValidPath(Mover->GetActorLocation(), Goal->GetActorLocation());
}

void UARPG_DA_MoveToActor::WhenActionActived()
{
	Super::WhenActionActived();
	ACharacterBase* Character = Cast<ACharacterBase>(Mover.Get());
	UARPG_MoveUtils::ARPG_MoveToActor(Character, Goal.Get(), FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_MoveToActor::WhenRequestFinished), AcceptRadius, false);
}

bool UARPG_DA_MoveToLocation::IsActionValid() const
{
	return Super::CanActiveAction() && IsExistValidPath(Mover->GetActorLocation(), Destination);
}

void UARPG_DA_MoveToLocation::WhenActionActived()
{
	Super::WhenActionActived();
	ACharacterBase* Character = Cast<ACharacterBase>(Mover.Get());
	UARPG_MoveUtils::ARPG_MoveToLocation(Character, Destination, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_MoveToLocation::WhenRequestFinished), AcceptRadius, false);
}
