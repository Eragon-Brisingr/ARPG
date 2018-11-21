// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BattleStyleSystemNormal.h"
#include "GameFramework/Actor.h"

FVector UARPG_BattleStyleSystemNormal::GetBestBattleLocation(class AActor* Enemy) const
{
	return Enemy->GetActorLocation();
}

FVector UARPG_BattleStyleSystemNormal::GetBestSearchLocation(const FVector& PotentialLocation) const
{
	if (PotentialLocation != FAISystem::InvalidLocation)
	{
		return PotentialLocation;
	}
	else
	{
		return FVector::ZeroVector;
	}
}
