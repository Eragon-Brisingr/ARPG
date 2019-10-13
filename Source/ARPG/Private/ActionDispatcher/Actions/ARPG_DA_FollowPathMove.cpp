// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_DA_FollowPathMove.h"
#include "ARPG_NavPath.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_AD_CharacterBase.h"
#include "CharacterBase.h"

UARPG_DA_FollowPathMove::UARPG_DA_FollowPathMove()
{
	FollowPathMoveConfig = UARPG_FollowPathMoveConfigBase::StaticClass();
}

TSet<AActor*> UARPG_DA_FollowPathMove::GetAllRegistableEntities() const
{
	return { Mover.Get(), Path.Get() };
}

bool UARPG_DA_FollowPathMove::IsActionValid() const
{
	return Mover.IsValid() && Path.IsValid();
}

void UARPG_DA_FollowPathMove::WhenActionActived()
{
	ACharacterBase* Character = Mover.Get();
	TargetPointIndex = FollowPathMoveConfig.GetDefaultObject()->GetStartFollowPathMoveIndex(Character, Path.Get(), TargetPointIndex);
	TargetPointIndex %= Path->NavPathPoints.Num();

	MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
}

void UARPG_DA_FollowPathMove::WhenActionAborted()
{
	ACharacterBase* Character = Mover.Get();
	if (CurBehavior)
	{
		CurBehavior->AbortDispatch(FOnDispatcherAbortedNative::CreateWeakLambda(Character, [=]()
			{
				CurBehavior = nullptr;
				DeactiveAction();
			}));
	}
	else
	{
		Character->StopMovement();
	}
}

void UARPG_DA_FollowPathMove::WhenActionDeactived()
{

}

void UARPG_DA_FollowPathMove::WhenActionFinished()
{

}

void UARPG_DA_FollowPathMove::MoveToNavPoint(const FARPG_NavPathPoint& NavPoint, ACharacterBase* Character)
{
	if (NavPoint.bAttachToRotation)
	{
		UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(Character, NavPoint.Location, NavPoint.Rotation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_FollowPathMove::WhenMoveFinished), NavPoint.GetAcceptableRadius(), false, true, true, true, FilterClass, false);
	}
	else
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(Character, NavPoint.Location, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_FollowPathMove::WhenMoveFinished), NavPoint.GetAcceptableRadius(), false, true, true, true, FilterClass, false);
	}
}

void UARPG_DA_FollowPathMove::WhenMoveFinished(const FPathFollowingResult& Result)
{
	switch (Result.Code)
	{
	case EPathFollowingResult::Aborted:
		return;
	case EPathFollowingResult::Invalid:
	case EPathFollowingResult::OffPath:
		ExecuteEventAndFinishAction(WhenCanNotReached);
		return;
	case EPathFollowingResult::Success:
	default:
		ACharacterBase* Character = Mover.Get();

		FARPG_NavPathPoint& CurPoint = Path->NavPathPoints[TargetPointIndex];
		const bool IsReached = bMoveToStart ? TargetPointIndex == 0 : TargetPointIndex == Path->NavPathPoints.Num() - 1;
		if (IsReached)
		{
			TargetPointIndex = INDEX_NONE;
			if (CurPoint.BehaviorTemplate)
			{
				UARPG_AD_CharacterBase* Behavior = CurPoint.GetBehavior(Character);
				Behavior->ActionDispatcherLeader = GetOwner();
				if (Behavior->CanStartDispatcher())
				{
					CurBehavior = Behavior;
					Behavior->StartDispatchWithEvent(FOnDispatchDeactiveNative::CreateWeakLambda(Character, [=](bool IsFinsihedCompleted)
						{
							CurBehavior = nullptr;
							Behavior->ActionDispatcherLeader = nullptr;
							ExecuteEventAndFinishAction(WhenReached);
						}));
				}
				else
				{
					Behavior->ActionDispatcherLeader = nullptr;
					ExecuteEventAndFinishAction(WhenReached);
				}
			}
			else
			{
				ExecuteEventAndFinishAction(WhenReached);
			}
		}
		else
		{
			TargetPointIndex += bMoveToStart ? -1 : 1;
			if (CurPoint.BehaviorTemplate)
			{
				UARPG_AD_CharacterBase* Behavior = CurPoint.GetBehavior(Character);
				Behavior->ActionDispatcherLeader = GetOwner();
				if (Behavior->CanStartDispatcher())
				{
					CurBehavior = Behavior;
					Behavior->StartDispatchWithEvent(FOnDispatchDeactiveNative::CreateWeakLambda(Character, [=](bool IsFinsihedCompleted)
						{
							CurBehavior = nullptr;
							Behavior->ActionDispatcherLeader = nullptr;
							MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
						}));
				}
				else
				{
					Behavior->ActionDispatcherLeader = nullptr;
					MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
				}
			}
			else
			{
				MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
			}
		}
	}
}

UARPG_DA_FollowPathPatrol::UARPG_DA_FollowPathPatrol()
{
	FollowPathMoveConfig = UARPG_FollowPathMoveConfigBase::StaticClass();
}

TSet<AActor*> UARPG_DA_FollowPathPatrol::GetAllRegistableEntities() const
{
	return { Mover.Get(), Path.Get() };
}

bool UARPG_DA_FollowPathPatrol::IsActionValid() const
{
	return Mover.IsValid() && Path.IsValid();
}

void UARPG_DA_FollowPathPatrol::WhenActionActived()
{
	ACharacterBase* Character = Mover.Get();
	TargetPointIndex = FollowPathMoveConfig.GetDefaultObject()->GetStartFollowPathMoveIndex(Character, Path.Get(), TargetPointIndex);
	TargetPointIndex %= Path->NavPathPoints.Num();

	MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
}

void UARPG_DA_FollowPathPatrol::WhenActionAborted()
{
	ACharacterBase* Character = Mover.Get();
	if (CurBehavior)
	{
		CurBehavior->AbortDispatch(FOnDispatcherAbortedNative::CreateWeakLambda(Character, [=]()
			{
				CurBehavior = nullptr;
				DeactiveAction();
			}));
	}
	else
	{
		Character->StopMovement();
	}
}

void UARPG_DA_FollowPathPatrol::WhenActionDeactived()
{

}

void UARPG_DA_FollowPathPatrol::WhenActionFinished()
{

}

void UARPG_DA_FollowPathPatrol::MoveToNavPoint(const FARPG_NavPathPoint& NavPoint, ACharacterBase* Character)
{
	if (NavPoint.bAttachToRotation)
	{
		UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(Character, NavPoint.Location, NavPoint.Rotation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_FollowPathPatrol::WhenMoveFinished), NavPoint.GetAcceptableRadius(), false, true, true, true, FilterClass, false);
	}
	else
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(Character, NavPoint.Location, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_DA_FollowPathPatrol::WhenMoveFinished), NavPoint.GetAcceptableRadius(), false, true, true, true, FilterClass, false);
	}
}

void UARPG_DA_FollowPathPatrol::WhenMoveFinished(const FPathFollowingResult& Result)
{
	switch (Result.Code)
	{
	case EPathFollowingResult::Aborted:
		return;
	case EPathFollowingResult::Invalid:
	case EPathFollowingResult::OffPath:
		ExecuteEventAndFinishAction(WhenCanNotReached);
		return;
	case EPathFollowingResult::Success:
	default:
		ACharacterBase* Character = Mover.Get();

		FARPG_NavPathPoint& CurPoint = Path->NavPathPoints[TargetPointIndex];
		if (Path->bIsClosedLoop)
		{
			TargetPointIndex += 1;
			TargetPointIndex %= Path->NavPathPoints.Num();
		}
		else
		{
			if (TargetPointIndex == 0)
			{
				bMoveReverse = false;
			}
			else if (TargetPointIndex == Path->NavPathPoints.Num() - 1)
			{
				bMoveReverse = true;
			}
			TargetPointIndex += bMoveReverse ? -1 : 1;
		}

		if (CurPoint.BehaviorTemplate)
		{
			UARPG_AD_CharacterBase* Behavior = CurPoint.GetBehavior(Character);
			Behavior->ActionDispatcherLeader = GetOwner();
			if (Behavior->CanStartDispatcher())
			{
				CurBehavior = Behavior;
				Behavior->StartDispatchWithEvent(FOnDispatchDeactiveNative::CreateWeakLambda(Character, [=](bool IsFinsihedCompleted)
					{
						CurBehavior = nullptr;
						Behavior->ActionDispatcherLeader = nullptr;
						MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
					}));
			}
			else
			{
				Behavior->ActionDispatcherLeader = nullptr;
				MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
			}
		}
		else
		{
			MoveToNavPoint(Path->NavPathPoints[TargetPointIndex], Character);
		}
	}
}
