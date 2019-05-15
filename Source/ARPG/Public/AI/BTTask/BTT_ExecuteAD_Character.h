// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ExecuteAD_Character.generated.h"

class UARPG_AD_CharacterBase;
class ACharacterBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "启动角色行为调度"))
class ARPG_API UBTT_ExecuteAD_Character : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced, Category = "行为调度器")
	UARPG_AD_CharacterBase* Dispatcher;

	UPROPERTY(Transient)
	TMap<ACharacterBase*, UARPG_AD_CharacterBase*> DispatherMap;
public:
	FString GetStaticDescription() const override;
protected:
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
