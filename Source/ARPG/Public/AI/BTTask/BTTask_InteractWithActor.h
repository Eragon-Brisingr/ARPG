// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_InteractWithActor.generated.h"

class AARPG_InteractableActorBase;

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
	TSoftObjectPtr<AARPG_InteractableActorBase> InteractableActor;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void WhenInteractFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp);
	void WhenInteractAbortFinished(UBehaviorTreeComponent* OwnerComp);
};
