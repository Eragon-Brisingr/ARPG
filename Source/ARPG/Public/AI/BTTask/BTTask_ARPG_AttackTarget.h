// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ARPG_AttackTarget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "攻击目标"))
class ARPG_API UBTTask_ARPG_AttackTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ARPG_AttackTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void WhenAttackFinished();
};