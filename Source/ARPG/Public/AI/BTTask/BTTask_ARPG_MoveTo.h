// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_ARPG_MoveTo.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UBTTask_ARPG_MoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UBTTask_ARPG_MoveTo()
	{
		bObserveBlackboardValue = true;
	}

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
