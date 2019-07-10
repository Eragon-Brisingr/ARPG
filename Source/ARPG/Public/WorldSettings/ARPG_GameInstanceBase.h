// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine/GameInstance.h>
#include "XD_SaveGameSystemInterface.h"
#include "ARPG_GameInstanceBase.generated.h"

class UARPG_SaveGameSystem;

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_GameInstanceBase : public UGameInstance, public IXD_SaveGame_GameInstanceInterface
{
	GENERATED_BODY()
public:
	UARPG_GameInstanceBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "游戏|读档", Instanced)
	UARPG_SaveGameSystem* SaveGameSystem;
	
	virtual class UXD_SaveGameSystemBase* GetSaveGameSystem_Implementation() const override;
};
