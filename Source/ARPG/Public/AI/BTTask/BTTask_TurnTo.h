// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_TurnTo.generated.h"

/**
 * 
 */
struct FARPG_TurnToMemory
{
	class UARPG_CharacterBehaviorBase* CurTurnToBehavior;
};

UCLASS(meta = (DisplayName = "转向"))
class ARPG_API UBTTask_TurnTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_TurnTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FARPG_TurnToMemory); }

	void WhenTurnToFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp, FARPG_TurnToMemory* NodeMemory);

	void WhenBehaviorAborted(UBehaviorTreeComponent* OwnerComp, FARPG_TurnToMemory* NodeMemory);
};
