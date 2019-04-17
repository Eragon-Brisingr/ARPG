// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_TimeManager.h"
#include "ARPG_TimeManager.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_TimeManager : public UXD_TimeManager
{
	GENERATED_BODY()
	
public:
	static UARPG_TimeManager* GetARPG_TimeManager(const UObject* WorldContextObject);
	
	
};
