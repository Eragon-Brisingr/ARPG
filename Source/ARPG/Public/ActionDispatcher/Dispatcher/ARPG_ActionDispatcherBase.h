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
};
