// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ARPG_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_GameStateBase : public AGameState
{
	GENERATED_BODY()
public:
	AARPG_GameStateBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "环境系统")
	class UXD_EnvironmentManager* EnvironmentManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "时间系统")
	class UXD_TimeManager* TimeManager;
	
};
