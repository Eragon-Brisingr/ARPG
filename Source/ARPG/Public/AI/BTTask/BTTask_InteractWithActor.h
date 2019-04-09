// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InteractWithActor.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "与物体交互"))
class ARPG_API UBTTask_InteractWithActor : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_InteractWithActor();

	UPROPERTY(EditAnywhere, Category = "交互")
	TSoftObjectPtr<AActor> InteractableActor;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
