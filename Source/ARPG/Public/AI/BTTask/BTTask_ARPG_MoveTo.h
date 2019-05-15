// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_ARPG_MoveTo.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "移动至"))
class ARPG_API UBTTask_ARPG_MoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UBTTask_ARPG_MoveTo()
	{
		bObserveBlackboardValue = true;
	}

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS(meta = (DisplayName = "移动至对象"))
class ARPG_API UBTTask_ARPG_MoveToActor : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ARPG_MoveToActor();

	UPROPERTY(EditAnywhere, Category = "配置")
	TSoftObjectPtr<AActor> Goal;
	UPROPERTY(EditAnywhere, Category = "配置")
	uint8 bUseRotation : 1;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS(meta = (DisplayName = "移动至目的地"))
class ARPG_API UBTTask_ARPG_MoveToLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ARPG_MoveToLocation();

	UPROPERTY(EditAnywhere, Category = "配置")
	TSoftObjectPtr<AActor> Goal;
	UPROPERTY(EditAnywhere, Category = "配置")
	uint8 bUseRotation : 1;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

