// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "XD_TimeSystem_GameStateInterface.h"
#include "XD_EnvironmentGameStateInterface.h"
#include "XD_CampSystemInterface.h"
#include "XD_SaveGameInterface.h"
#include "XD_ActionDispatcherGameStateImpl.h"
#include "ARPG_GameStateBase.generated.h"

class UXD_ActionDispatcherManager;
class UARPG_EnvironmentManager;
class UARPG_TimeManager;
class UARPG_CampManager;

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_GameStateBase : public AGameState, 
	public IXD_SaveGameInterface,
	public IXD_TimeSystem_GameStateInterface, 
	public IXD_Environment_GameStateInterface,
	public IXD_CampSystem_GameStateInterface,
	public IXD_ActionDispatcherGameStateImpl
{
	GENERATED_BODY()
public:
	AARPG_GameStateBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "环境系统")
	UARPG_EnvironmentManager* EnvironmentManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "时间系统")
	UARPG_TimeManager* TimeManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "阵营系统")
	UARPG_CampManager* CampManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "行为系统")
	UXD_ActionDispatcherManager* ActionDispatcherManager;

	UXD_TimeManager* GetGameTimeManager_Implementation() const override;

	UXD_EnvironmentManager* GetEnvironmentManager_Implementation() const override;

	UXD_CampManager* GetCampManager_Implementation() const override;

	UXD_ActionDispatcherManager* GetActionDispatcherManager_Implementation() const override;
};
