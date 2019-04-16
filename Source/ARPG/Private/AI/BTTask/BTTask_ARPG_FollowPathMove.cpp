// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ARPG_FollowPathMove.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_NavPath.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "ARPG_AD_CharacterBase.h"

UBTTask_ARPG_FollowPathMove::UBTTask_ARPG_FollowPathMove()
{
	FollowPathMoveConfig = UARPG_FollowPathMoveConfigBase::StaticClass();

	NodeName = TEXT("沿路径移动");
}

EBTNodeResult::Type UBTTask_ARPG_FollowPathMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Path && FollowPathMoveConfig)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			FARPG_FollowPathMoveMemory* FollowPathMoveMemory = reinterpret_cast<FARPG_FollowPathMoveMemory*>(NodeMemory);

			FollowPathMoveMemory->TargetPointIndex = FollowPathMoveConfig.GetDefaultObject()->GetStartFollowPathMoveIndex(Character, Path.Get(), FollowPathMoveMemory->TargetPointIndex);
			FollowPathMoveMemory->TargetPointIndex %= Path->NavPathPoints.Num();

			MoveToNavPoint(Path->NavPathPoints[FollowPathMoveMemory->TargetPointIndex], Character, OwnerComp, FollowPathMoveMemory);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_ARPG_FollowPathMove::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	MyController->StopMovement();

	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		FARPG_FollowPathMoveMemory* FollowPathMoveMemory = reinterpret_cast<FARPG_FollowPathMoveMemory*>(NodeMemory);
		if (UARPG_AD_CharacterBase* CurBehavior = FollowPathMoveMemory->CurBehavior)
		{
			CurBehavior->AbortDispatch(FOnDispatcherAbortedNative::CreateWeakLambda(Character, [=, P_OwnerComp = &OwnerComp]()
				{
					FARPG_FollowPathMoveMemory* FollowPathMoveMemory = reinterpret_cast<FARPG_FollowPathMoveMemory*>(NodeMemory);
					FollowPathMoveMemory->CurBehavior = nullptr;
					FinishLatentAbort(*P_OwnerComp);
				}));
		}
		else
		{
			Character->StopMovement();
		}
	}
	return EBTNodeResult::Aborted;
}

void UBTTask_ARPG_FollowPathMove::WhenMoveFinished(const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp, FARPG_FollowPathMoveMemory* FollowPathMoveMemory)
{
	if (Path)
	{
		switch (Result.Code)
		{
		case EPathFollowingResult::Aborted:
		case EPathFollowingResult::Invalid:
			FollowPathMoveMemory->CurBehavior = nullptr;
			return;

		case EPathFollowingResult::Success:
		case EPathFollowingResult::OffPath:
		default:
			AAIController * MyController = OwnerComp->GetAIOwner();
			if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
			{
				int32 CurIndex = FollowPathMoveMemory->TargetPointIndex;
				if (Path->bIsClosedLoop)
				{
					FollowPathMoveMemory->TargetPointIndex += 1;
					FollowPathMoveMemory->TargetPointIndex %= Path->NavPathPoints.Num();
				}
				else
				{
					if (FollowPathMoveMemory->TargetPointIndex == 0)
					{
						FollowPathMoveMemory->bMoveReverse = false;
					}
					else if (FollowPathMoveMemory->TargetPointIndex == Path->NavPathPoints.Num() - 1)
					{
						FollowPathMoveMemory->bMoveReverse = true;
					}
					FollowPathMoveMemory->TargetPointIndex += FollowPathMoveMemory->bMoveReverse ? -1 : 1;
				}

				FARPG_NavPathPoint& CurPoint = Path->NavPathPoints[CurIndex];
				UARPG_AD_CharacterBase* Behavior = CurPoint.GetBehavior(Character);
				if (Behavior && Behavior->CanStartDispatcher())
				{
					Behavior->StartDispatchWithEvent(FOnDispatchDeactiveNative::CreateWeakLambda(Character, [=](bool IsFinsihedCompleted)
						{
							AAIController * MyController = OwnerComp->GetAIOwner();
							if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
							{
								if (Path)
								{
									FollowPathMoveMemory->CurBehavior = nullptr;
									MoveToNavPoint(Path->NavPathPoints[FollowPathMoveMemory->TargetPointIndex], Character, *OwnerComp, FollowPathMoveMemory);
								}
								else
								{
									FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
								}
							}
						}));
					FollowPathMoveMemory->CurBehavior = Behavior;
				}
				else
				{
					MoveToNavPoint(Path->NavPathPoints[FollowPathMoveMemory->TargetPointIndex], Character, *OwnerComp, FollowPathMoveMemory);
				}
			}
		}
	}
	else
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_ARPG_FollowPathMove::MoveToNavPoint(const FARPG_NavPathPoint &NavPoint, ACharacterBase* Character, UBehaviorTreeComponent& OwnerComp, FARPG_FollowPathMoveMemory* FollowPathMoveMemory)
{
	if (NavPoint.bAttachToRotation)
	{
		UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(Character, NavPoint.Location, NavPoint.Rotation, FOnARPG_MoveFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenMoveFinished, &OwnerComp, FollowPathMoveMemory), NavPoint.GetAcceptableRadius(), false, true, true, true, FilterClass, false);
	}
	else
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(Character, NavPoint.Location, FOnARPG_MoveFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenMoveFinished, &OwnerComp, FollowPathMoveMemory), NavPoint.GetAcceptableRadius(), false, true, true, true, FilterClass, false);
	}
}

int32 UARPG_FollowPathMoveConfigBase::GetStartFollowPathMoveIndex(ACharacterBase* Mover, class AARPG_NavPath* Path, int32 RememberIndex) const
{
	return RememberIndex != -1 ? RememberIndex : 0;
}
