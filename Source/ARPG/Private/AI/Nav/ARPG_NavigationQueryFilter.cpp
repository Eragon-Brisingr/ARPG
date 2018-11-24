// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_NavigationQueryFilter.h"

TSubclassOf<UNavigationQueryFilter> UARPG_NavigationQueryFilter::GetSimpleFilterForAgent(const UObject& Querier) const
{
	return UARPG_NavigationQueryFilter::StaticClass();
}
