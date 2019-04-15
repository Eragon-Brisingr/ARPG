// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_InteractWithActor.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "ARPG_AI_Log.h"
#include "XD_DebugFunctionLibrary.h"

UBTTask_InteractWithActor::UBTTask_InteractWithActor()
{
	NodeName = TEXT("与物体交互");
}

EBTNodeResult::Type UBTTask_InteractWithActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (InteractableActor.IsValid())
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			if (Character->CanInteractWithTarget(InteractableActor.Get()))
			{
				AI_Display_VLog(Character, "请求与%s进行交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractableActor.Get()));
				Character->InvokeInteractWithEndEvent(InteractableActor.Get(), FOnInteractEnd::CreateWeakLambda(this, [=, P_OwnerComp = &OwnerComp](EInteractEndResult Result)
					{
						AI_Display_VLog(Character, "与%s进行交互结束，结果为%s", *UXD_DebugFunctionLibrary::GetDebugName(InteractableActor.Get()), Result == EInteractEndResult::Succeed ? TEXT("成功") : TEXT("失败"));
						FinishLatentTask(*P_OwnerComp, Result == EInteractEndResult::Succeed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
					}));
				return EBTNodeResult::InProgress;
			}
		}
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
			AI_Display_VLog(Character, "中断与%s进行交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractableActor.Get()));
			Character->InvokeAbortInteractWithAbortEvent(FOnInteractAbortEnd::CreateWeakLambda(this, [this, P_OwnerComp = &OwnerComp]()
				{
					FinishLatentAbort(*P_OwnerComp);
				}));
		}
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Aborted;
}

void UBTTask_InteractWithActor::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
// 	if (InteractableActor)
// 	{
// 		AAIController* MyController = OwnerComp.GetAIOwner();
// 		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
// 		{
// 			InteractableActor->StartInteract(Character, FOnInteractFinished::CreateUObject(this, &UBTTask_InteractWithActor::WhenInteractEnd, &OwnerComp));
// 		}
// 	}
}
