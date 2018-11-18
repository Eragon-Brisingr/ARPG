// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ARPG_MoveTo.h"
#include "CharacterBase.h"
#include "AIController.h"

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
