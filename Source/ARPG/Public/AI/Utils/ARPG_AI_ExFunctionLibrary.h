// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SubclassOf.h"
#include "ARPG_AI_ExFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_AI_ExFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//修改完别忘记调用RequestStimuliListenerUpdate
	UFUNCTION(BlueprintCallable, Category = "AI|Perception")
	static class UAISenseConfig* GetSenseConfig(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> AISenseType);
};
