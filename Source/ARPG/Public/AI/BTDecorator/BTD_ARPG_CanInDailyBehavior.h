// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_ARPG_CanInDailyBehavior.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "能执行日常行为"))
class ARPG_API UBTD_ARPG_CanInDailyBehavior : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTD_ARPG_CanInDailyBehavior();

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	FString GetStaticDescription() const override;
};
