// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "ARPG_NavigationQueryFilter.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_NavigationQueryFilter : public UNavigationQueryFilter
{
	GENERATED_BODY()
public:
	virtual TSubclassOf<UNavigationQueryFilter> GetSimpleFilterForAgent(const UObject& Querier) const;
};
