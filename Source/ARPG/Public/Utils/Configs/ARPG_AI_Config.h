// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_AI_Config.generated.h"

class UCurveFloat;

/**
 * 
 */
UCLASS(Config = "ARPG_AI_Config", defaultconfig)
class ARPG_API UARPG_AI_Config : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "AI", Config)
	TSoftObjectPtr<UCurveFloat> DefaultSightAlertCurve;

	UPROPERTY(EditAnywhere, Category = "AI", Config)
	TSoftObjectPtr<UCurveFloat> DefaultHearAlertCurve;

	UPROPERTY(EditAnywhere, Category = "AI", Config)
	TSoftObjectPtr<UCurveFloat> DefaultNoseAlertCurve;
};
