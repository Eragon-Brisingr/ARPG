﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_InteractWithActor.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "ARPG_InteractableActor.h"

UBTTask_InteractWithActor::UBTTask_InteractWithActor()
{
	NodeName = TEXT("与物体交互");
}

EBTNodeResult::Type UBTTask_InteractWithActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (InteractableActor)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			InteractableActor->StartInteract(Character, FOnInteractFinished::CreateUObject(this, &UBTTask_InteractWithActor::WhenInteractFinished, &OwnerComp));
		}
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_InteractWithActor::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (InteractableActor)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			InteractableActor->EndInteract(Character, FOnInteractAbortFinished::CreateUObject(this, &UBTTask_InteractWithActor::WhenInteractAbortFinished, &OwnerComp));
		}
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Aborted;
}

void UBTTask_InteractWithActor::WhenInteractFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, Succeed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTTask_InteractWithActor::WhenInteractAbortFinished(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentAbort(*OwnerComp);
}
