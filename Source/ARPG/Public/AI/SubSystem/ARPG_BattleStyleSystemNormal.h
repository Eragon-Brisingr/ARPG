// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SubSystem/ARPG_BattleStyleSystemBase.h"
#include "AITypes.h"
#include "ARPG_BattleStyleSystemNormal.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_BattleStyleSystemNormal : public UARPG_BattleStyleSystemBase
{
	GENERATED_BODY()
public:
	virtual FVector GetBestBattleLocation(class AActor* Enemy) const override;

	virtual FVector GetBestSearchLocation(const FVector& PotentialLocation) const override;
};
