// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SubclassOf.h"
#include "GameplayTagContainer.h"
#include "ARPG_AI_ExFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_AI_ExFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AI|Logic")
	static void SetDynamicSubtree(class AAIController* AIController, FGameplayTag InjectTag, class UBehaviorTree* BehaviorAsset);
};
