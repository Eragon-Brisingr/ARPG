// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ARPG_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_GameModeBase : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
