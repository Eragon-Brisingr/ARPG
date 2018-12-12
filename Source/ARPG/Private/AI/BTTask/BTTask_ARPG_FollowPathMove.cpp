// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ARPG_FollowPathMove.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_NavPath.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_ActorFunctionLibrary.h"

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
			UARPG_MoveUtils::ARPG_MoveToLocation(Character, Path->NavPathPoints[FollowPathMoveMemory->TargetPointIndex].Location, FOnARPG_MoveFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenMoveFinished, &OwnerComp, FollowPathMoveMemory), AcceptableRadius, false, true, true, true, FilterClass, false);
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
		if (UARPG_CharacterBehaviorBase* CurBehavior = FollowPathMoveMemory->CurBehavior)
		{
			CurBehavior->AbortBehavior(Character, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenBehaviorAborted, &OwnerComp, NodeMemory));
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

				const FARPG_NavPathPoint& CurPoint = Path->NavPathPoints[CurIndex];
				if (CurPoint.Behavior && Character->CanPlayTurnMontage())
				{
					if (CurPoint.bAttachToRotation && Character->CharacterTurnAction)
					{
						if (CurPoint.bAttachToLocation)
						{
							UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Character, CurPoint.Location, 1.f);
						}
						FollowPathMoveMemory->CurBehavior = Character->TurnTo(CurPoint.Rotation, FOnCharacterBehaviorFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenTurnFinished, Character, CurPoint, OwnerComp, FollowPathMoveMemory));
					}
					else
					{
						WhenTurnFinished(true, Character, CurPoint, OwnerComp, FollowPathMoveMemory);
					}
				}
				else
				{
					UARPG_MoveUtils::ARPG_MoveToLocation(Character, Path->NavPathPoints[FollowPathMoveMemory->TargetPointIndex].Location, FOnARPG_MoveFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenMoveFinished, OwnerComp, FollowPathMoveMemory), AcceptableRadius, false, true, true, true, FilterClass, false);
				}
			}
		}
	}
	else
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	}
}

void UBTTask_ARPG_FollowPathMove::WhenTurnFinished(bool Succeed, ACharacterBase* Character, FARPG_NavPathPoint CurPoint, UBehaviorTreeComponent* OwnerComp, FARPG_FollowPathMoveMemory* FollowPathMoveMemory)
{
	FollowPathMoveMemory->CurBehavior = CurPoint.WorldPositionExecuteBehavior(Character, FOnCharacterBehaviorFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenBehaviorFinished, OwnerComp, FollowPathMoveMemory));
}

void UBTTask_ARPG_FollowPathMove::WhenBehaviorFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp, FARPG_FollowPathMoveMemory* FollowPathMoveMemory)
{
	AAIController * MyController = OwnerComp->GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		if (Path)
		{
			FollowPathMoveMemory->CurBehavior = nullptr;
			UARPG_MoveUtils::ARPG_MoveToLocation(Character, Path->NavPathPoints[FollowPathMoveMemory->TargetPointIndex].Location, FOnARPG_MoveFinished::CreateUObject(this, &UBTTask_ARPG_FollowPathMove::WhenMoveFinished, OwnerComp, FollowPathMoveMemory), AcceptableRadius, false, true, true, true, FilterClass, false);
		}
		else
		{
			FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		}
	}
}

void UBTTask_ARPG_FollowPathMove::WhenBehaviorAborted(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	FARPG_FollowPathMoveMemory* FollowPathMoveMemory = reinterpret_cast<FARPG_FollowPathMoveMemory*>(NodeMemory);
	FollowPathMoveMemory->CurBehavior = nullptr;
	FinishLatentAbort(*OwnerComp);
}

int32 UARPG_FollowPathMoveConfigBase::GetStartFollowPathMoveIndex(ACharacterBase* Mover, class AARPG_NavPath* Path, int32 RememberIndex) const
{
	return RememberIndex != -1 ? RememberIndex : 0;
}
