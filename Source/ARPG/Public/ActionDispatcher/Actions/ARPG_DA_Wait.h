// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "XD_TimeSystemType.h"
#include "ARPG_DA_Wait.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "等待"))
class ARPG_API UARPG_DA_Wait : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	bool IsActionValid() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;

protected:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<APawn> Pawn;
};

UCLASS(meta = (DisplayName = "等待一段时间"))
class ARPG_API UARPG_DA_WaitGameTime : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	bool IsActionValid() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;

protected:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<APawn> Pawn;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FXD_GameTimeSpan GameTimeSpan;

	UPROPERTY(SaveGame, meta = (DisplayName = "当等待结束"))
	FOnDispatchableActionFinishedEvent WhenWaitFinished;

	FXD_NativeSpecialGameTimeHandle GameTimeHandle;
};

