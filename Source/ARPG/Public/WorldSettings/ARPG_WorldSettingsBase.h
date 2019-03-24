// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "XD_TimeSystemType.h"
#include "XD_SaveGameInterface.h"
#include "ARPG_WorldSettingsBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_WorldSettingsBase : public AWorldSettings, 
	public IXD_SaveGameInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "时间", meta = (DisplayName = "游戏开始时间"))
	FXD_GameTime StartGameTime;

	UPROPERTY(EditAnywhere, Category = "时间", meta = (DisplayName = "时间流逝速度"))
	float TimeSpendRate = 20.f;

	void WhenGameInit_Implementation() override;
	bool NeedSave_Implementation() const override;

	bool IsMainWorldSettings() const;
};
