// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_ARPG_CharacterCondition.generated.h"

class UARPG_CharacterConditionBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "角色条件"))
class ARPG_API UBTD_ARPG_CharacterCondition : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTD_ARPG_CharacterCondition();

	UPROPERTY(EditAnywhere, Category = "条件")
	TSubclassOf<UARPG_CharacterConditionBase> Condition;

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	FString GetStaticDescription() const override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
