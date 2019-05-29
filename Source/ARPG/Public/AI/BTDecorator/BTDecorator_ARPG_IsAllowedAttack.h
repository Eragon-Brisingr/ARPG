// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_ARPG_IsAllowedAttack.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "是否允许攻击"))
class ARPG_API UBTDecorator_ARPG_IsAllowedAttack : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_ARPG_IsAllowedAttack();

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
