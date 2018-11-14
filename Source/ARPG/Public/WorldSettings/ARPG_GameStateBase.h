// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "XD_TimeSystem_GameStateInterface.h"
#include "XD_EnvironmentGameStateInterface.h"
#include "XD_CampSystemInterface.h"
#include "XD_SaveGameInterface.h"
#include "ARPG_GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_GameStateBase : public AGameState, 
	public IXD_SaveGameInterface,
	public IXD_TimeSystem_GameStateInterface, 
	public IXD_Environment_GameStateInterface,
	public IXD_CampSystem_GameStateInterface
{
	GENERATED_BODY()
public:
	AARPG_GameStateBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "环境系统")
	class UARPG_EnvironmentManager* EnvironmentManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "时间系统")
	class UARPG_TimeManager* TimeManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "阵营系统")
	class UARPG_CampManager* CampManager;

	class UXD_TimeManager* GetGameTimeManager_Implementation() const override;

	class UXD_EnvironmentManager* GetEnvironmentManager_Implementation() const override;

	virtual class UXD_CampManager* GetCampManager_Implementation() const override;
};
