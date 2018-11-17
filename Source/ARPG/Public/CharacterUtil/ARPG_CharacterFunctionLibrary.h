// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARPG_CharacterFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_CharacterFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "角色|感知")
	static bool IsInDamageArea(class ACharacterBase* Asker, float TraceRadius = 500.f);
	
	
};
