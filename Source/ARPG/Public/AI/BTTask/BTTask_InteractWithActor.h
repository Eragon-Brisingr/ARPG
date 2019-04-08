// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARPG_InteractType.h"
#include "BTTask_InteractWithActor.generated.h"

class AARPG_InteractableActorBase;
enum class EInteractEndResult : uint8;

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

	void WhenInteractEnd(EInteractEndResult Result, UBehaviorTreeComponent* OwnerComp);
	void WhenInteractAbortFinished(UBehaviorTreeComponent* OwnerComp);
protected:
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
