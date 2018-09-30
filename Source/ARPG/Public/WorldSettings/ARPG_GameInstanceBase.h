// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine/GameInstance.h>
#include "XD_SaveGameSystemInterface.h"
#include "ARPG_GameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_GameInstanceBase : public UGameInstance, public IXD_SaveGame_GameInstanceInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏|读档", Instanced)
	class UXD_SaveGameSystemBase* SaveGameSystem;
	
	virtual class UXD_SaveGameSystemBase* GetSaveGameSystem_Implementation() const override { return SaveGameSystem; }
};
