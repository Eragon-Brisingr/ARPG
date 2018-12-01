// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterBehaviorContainer.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "行为容器_随机选择"))
class ARPG_API UCBC_RandomSelect : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_RandomSelect();

	UPROPERTY(EditAnywhere, Category = "容器")
	TMap<class UARPG_CharacterBehaviorConfigBase*, float> RandomMap;
};

UCLASS()
class ARPG_API UCB_RandomSelect : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation) override;

	void AbortBehavior(class ACharacterBase* Executer) override;
public:
	const UCBC_RandomSelect* GetConfig() const;
};
