// Fill out your copyright notice in the Description page of Project Settings.
#include "BTT_ExecuteAD_Character.h"
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_AD_CharacterBase.h"
#include "AIController.h"
#include "CharacterBase.h"


FString UBTT_ExecuteAD_Character::GetStaticDescription() const
{
	return FString::Printf(TEXT("启动角色行为调度%s"), *UXD_DebugFunctionLibrary::GetDebugName(Dispatcher));
}

EBTNodeResult::Type UBTT_ExecuteAD_Character::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Dispatcher)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			UARPG_AD_CharacterBase*& RuntimeDispatcher = DispatherMap.FindOrAdd(Character);
			if (RuntimeDispatcher)
			{
				Character->bIsInBTreeDispatching = false;
				RuntimeDispatcher->AbortDispatch(FOnDispatcherAbortedNative::CreateWeakLambda(Character, [=, P_OwnerComp = &OwnerComp]()
					{
						FinishLatentAbort(*P_OwnerComp);
					}));
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type UBTT_ExecuteAD_Character::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (Dispatcher)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			UARPG_AD_CharacterBase*& RuntimeDispatcher = DispatherMap.FindOrAdd(Character);
			if (RuntimeDispatcher == nullptr)
			{
				RuntimeDispatcher = ::DuplicateObject(Dispatcher, Character);
				RuntimeDispatcher->Character = Character;
				RuntimeDispatcher->InitLeader(Character);
			}
			Character->bIsInBTreeDispatching = true;
			RuntimeDispatcher->StartDispatchWithEvent(FOnDispatchDeactiveNative::CreateWeakLambda(this, [=, P_OwnerComp = &OwnerComp](bool IsFinsihedCompleted)
				{
					FinishLatentTask(*P_OwnerComp, IsFinsihedCompleted ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
				}));
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}
