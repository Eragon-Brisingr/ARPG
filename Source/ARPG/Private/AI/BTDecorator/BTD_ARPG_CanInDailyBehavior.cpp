// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_ARPG_CanInDailyBehavior.h"
#include "AIController.h"
#include "CharacterBase.h"

UBTD_ARPG_CanInDailyBehavior::UBTD_ARPG_CanInDailyBehavior()
{
	FlowAbortMode = EBTFlowAbortMode::Self;
	bNotifyTick = true;
}

bool UBTD_ARPG_CanInDailyBehavior::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(AIOwner->GetPawn()))
	{
		return Character->CanInDailyBehavior();
	}
	return false;
}

void UBTD_ARPG_CanInDailyBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (FlowAbortMode != EBTFlowAbortMode::None && !CalculateRawConditionValue(OwnerComp, NodeMemory))
	{
		OwnerComp.RequestExecution(this);
	}
}

FString UBTD_ARPG_CanInDailyBehavior::GetStaticDescription() const
{
	return TEXT("能执行日常行为");
}
