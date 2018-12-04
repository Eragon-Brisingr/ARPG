// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CharacterBehavior.generated.h"

class AActor;

/**
 * 
 */
UCLASS(meta = (DisplayName = "角色行为"))
class ARPG_API UBTTask_CharacterBehavior : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_CharacterBehavior();

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	class UARPG_CharacterBehaviorConfigBase* Behavior;

	UPROPERTY(EditAnywhere, Category = "行为")
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "行为")
	FRotator Rotation;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "行为")
	TSoftObjectPtr<AActor> Target;

	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bRefresh : 1;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void WhenBehaviorFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp);
	void WhenBehaviorAbortFinished(UBehaviorTreeComponent* OwnerComp);
};
