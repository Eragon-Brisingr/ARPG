// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_CampInfo.h"
#include "ARPG_CampInfo.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_CampInfo : public UXD_CampInfo
{
	GENERATED_BODY()
public:
	UARPG_CampInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
