// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CharacterBehavior.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "ARPG_CharacterBehaviorBase.h"

#if WITH_EDITOR

UBTTask_CharacterBehavior::UBTTask_CharacterBehavior()
{
	NodeName = TEXT("角色行为");
}

void UBTTask_CharacterBehavior::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UBTTask_CharacterBehavior, Target) || PropertyName == GET_MEMBER_NAME_CHECKED(UBTTask_CharacterBehavior, bRefresh))
	{
		if (Target.IsValid())
		{
			Location = Target->GetActorLocation();
			Rotation = Target->GetActorRotation();
		}
	}
}
#endif // WITH_EDITOR

EBTNodeResult::Type UBTTask_CharacterBehavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Behavior)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			Behavior->ExecuteBehavior(Character, Location, Rotation, UARPG_CharacterBehaviorBase::FOnBehaviorFinished::CreateUObject(this, &UBTTask_CharacterBehavior::WhenBehaviorFinished, &OwnerComp));
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_CharacterBehavior::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Behavior)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			Behavior->AbortBehavior(Character, UARPG_CharacterBehaviorBase::FOnBehaviorAbortFinished::CreateUObject(this, &UBTTask_CharacterBehavior::WhenBehaviorAbortFinished, &OwnerComp));
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

void UBTTask_CharacterBehavior::WhenBehaviorFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, Succeed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTTask_CharacterBehavior::WhenBehaviorAbortFinished(UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentAbort(*OwnerComp);
}
