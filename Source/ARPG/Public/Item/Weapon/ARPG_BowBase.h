// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include "ARPG_BowBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_BowBase : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	AARPG_BowBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY()
	class AARPG_ArrowBase* HoldingArrow;

	float HoldingTime;
	
	void SpawnArrowInHand();

	void LaunchArrow();
};
