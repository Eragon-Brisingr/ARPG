// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARPG_AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_AIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	AARPG_AIControllerBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "角色|AI")
	class UARPG_AIPerceptionComponent* AIPerception;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
