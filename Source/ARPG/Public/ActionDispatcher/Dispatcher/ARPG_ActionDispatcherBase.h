// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ActionDispatcherBase.h"
#include "ARPG_ActionDispatcherBase.generated.h"

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "行为调度器"))
class ARPG_API UARPG_ActionDispatcherBase : public UXD_ActionDispatcherBase
{
	GENERATED_BODY()
public:
	UARPG_ActionDispatcherBase();

	bool ActionIsBothCompatible(UXD_DispatchableActionBase* LHS, UXD_DispatchableActionBase* RHS) const override;

	// 执行该调度的时候能否被交互
	// e.g. 睡觉时不能交互
	UPROPERTY(EditDefaultsOnly, Category = "行为")
	uint8 bInteractable : 1;
};
