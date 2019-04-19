// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnTo.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "ARPG_CharacterBehaviorBase.h"

UBTTask_TurnTo::UBTTask_TurnTo()
{
	NodeName = TEXT("转向");
}

EBTNodeResult::Type UBTTask_TurnTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		FARPG_TurnToMemory* TurnToMemory = reinterpret_cast<FARPG_TurnToMemory*>(NodeMemory);
		if (Character->CanTurnTo())
		{
			FRotator TargetWorldRotation;
			if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
			{
				TargetWorldRotation = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			{
				UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
				if (AActor* TargetActor = Cast<AActor>(KeyValue))
				{
					TargetWorldRotation = (TargetActor->GetActorLocation() - Character->GetActorLocation()).Rotation();
				}
				else
				{
					return EBTNodeResult::Failed;
				}
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
				TargetWorldRotation = (KeyValue - Character->GetActorLocation()).Rotation();
			}
			else
			{
				return EBTNodeResult::Failed;
			}


			Character->TurnTo(TargetWorldRotation, FOnCharacterBehaviorFinished::CreateUObject(this, &UBTTask_TurnTo::WhenTurnToFinished, &OwnerComp, TurnToMemory));
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_TurnTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	MyController->StopMovement();

	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		FARPG_TurnToMemory* TurnToMemory = reinterpret_cast<FARPG_TurnToMemory*>(NodeMemory);
		if (UARPG_CharacterBehaviorBase* CurTurnToBehavior = TurnToMemory->CurTurnToBehavior)
		{
			CurTurnToBehavior->AbortBehavior(Character, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UBTTask_TurnTo::WhenBehaviorAborted, &OwnerComp, TurnToMemory));
		}
	}
	return EBTNodeResult::Aborted;
}

void UBTTask_TurnTo::WhenTurnToFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp, FARPG_TurnToMemory* NodeMemory)
{
	NodeMemory->CurTurnToBehavior = nullptr;
	FinishLatentTask(*OwnerComp, Succeed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTTask_TurnTo::WhenBehaviorAborted(UBehaviorTreeComponent* OwnerComp, FARPG_TurnToMemory* NodeMemory)
{
	NodeMemory->CurTurnToBehavior = nullptr;
	FinishLatentAbort(*OwnerComp);
}
