// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ARPG_MoveTo.h"
#include "CharacterBase.h"
#include "AIController.h"
#include "ARPG_MoveUtils.h"

EBTNodeResult::Type UBTTask_ARPG_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* MyController = OwnerComp.GetAIOwner())
	{
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			FilterClass = Character->NavigationQueryFilter;
			Character->WhenReceivedMoveRequest();
		}
	}

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

UBTTask_ARPG_MoveToActor::UBTTask_ARPG_MoveToActor()
	:bUseRotation(true)
{

}

EBTNodeResult::Type UBTTask_ARPG_MoveToActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Goal)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			AActor* GoalActor = Goal.Get();
			FOnARPG_MoveFinished OnARPG_MoveFinished = FOnARPG_MoveFinished::CreateWeakLambda(this, [=, P_OwnerComp = &OwnerComp](const FPathFollowingResult& Result)
				{
					FinishLatentTask(*P_OwnerComp, Result.Code == EPathFollowingResult::Success ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
				});
			if (bUseRotation)
			{
				UARPG_MoveUtils::ARPG_MoveToActorAndTurn(Character, GoalActor, GoalActor, OnARPG_MoveFinished);
			}
			else
			{
				UARPG_MoveUtils::ARPG_MoveToActor(Character, GoalActor, OnARPG_MoveFinished);
			}
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_ARPG_MoveToActor::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Goal)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			Character->StopMovement();
			return EBTNodeResult::Aborted;
		}
	}
	return EBTNodeResult::Aborted;
}

UBTTask_ARPG_MoveToLocation::UBTTask_ARPG_MoveToLocation()
	:bUseRotation(true)
{

}

EBTNodeResult::Type UBTTask_ARPG_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Goal)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			AActor* GoalActor = Goal.Get();
			FOnARPG_MoveFinished OnARPG_MoveFinished = FOnARPG_MoveFinished::CreateWeakLambda(this, [=, P_OwnerComp = &OwnerComp](const FPathFollowingResult& Result)
				{
					FinishLatentTask(*P_OwnerComp, Result.Code == EPathFollowingResult::Success ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
				});
			if (bUseRotation)
			{
				UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(Character, GoalActor->GetActorLocation(), GoalActor->GetActorRotation(), OnARPG_MoveFinished);
			}
			else
			{
				UARPG_MoveUtils::ARPG_MoveToLocation(Character, GoalActor->GetActorLocation(), OnARPG_MoveFinished);
			}
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_ARPG_MoveToLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Goal)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			Character->StopMovement();
			return EBTNodeResult::Aborted;
		}
	}
	return EBTNodeResult::Aborted;
}
