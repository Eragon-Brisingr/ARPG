// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_DispatchableActionBase.h"
#include "ARPG_DA_ItemNode.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "移动物品"))
class ARPG_API UARPG_DA_MoveItem : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<ACharacterBase> Role;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<AActor> Other;

	UPROPERTY(SaveGame, meta = (DisplayName = "当移动道具结束"))
	FOnDispatchableActionFinishedEvent WhenMoveItemFinished;
};

UCLASS(meta = (DisplayName = "交易物品"))
class ARPG_API UARPG_DA_TradeItem : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<ACharacterBase> Role;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<AActor> Other;

	UPROPERTY(SaveGame, meta = (DisplayName = "当交易道具结束"))
	FOnDispatchableActionFinishedEvent WhenTradeItemFinished;
};
