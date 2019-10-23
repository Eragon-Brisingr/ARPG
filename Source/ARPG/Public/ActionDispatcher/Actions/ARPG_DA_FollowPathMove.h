// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_DispatchableActionBase.h"
#include "ARPG_NavPath.h"
#include "Navigation/PathFollowingComponent.h"
#include "ARPG_DA_FollowPathMove.generated.h"

class ACharacterBase;
class AARPG_NavPath;
class UARPG_FollowPathMoveConfigBase;
class UARPG_AD_CharacterBase;
class UNavigationQueryFilter;

/**
 * 
 */
UCLASS(meta = (DisplayName = "沿路径移动"))
class ARPG_API UARPG_DA_FollowPathMove : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	UARPG_DA_FollowPathMove();

	TSet<AActor*> GetAllRegistableEntities() const override;
	bool IsActionValid() const override;
	void WhenActionActived() override;
	void WhenActionAborted() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
public:
	UPROPERTY(SaveGame)
	int32 TargetPointIndex = INDEX_NONE;
	UPROPERTY()
	UARPG_AD_CharacterBase* CurBehavior;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "移动者"))
	TSoftObjectPtr<ACharacterBase> Mover;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "路径"))
	TSoftObjectPtr<AARPG_NavPath> Path;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "向起点移动"))
	uint8 bMoveToStart : 1;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "沿路径移动行为配置"))
	TSubclassOf<UARPG_FollowPathMoveConfigBase> FollowPathMoveConfig;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (DisplayName = "当到达了终点"))
	FOnDispatchableActionFinishedEvent WhenReached;

	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (DisplayName = "当无法到达"))
	FOnDispatchableActionFinishedEvent WhenCanNotReached;
private:
	void MoveToNavPoint(const FARPG_NavPathPoint& NavPoint, ACharacterBase* Character);
	void WhenMoveFinished(const FPathFollowingResult& Result);
};

UCLASS(meta = (DisplayName = "沿路径巡逻"))
class ARPG_API UARPG_DA_FollowPathPatrol : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	UARPG_DA_FollowPathPatrol();

	TSet<AActor*> GetAllRegistableEntities() const override;
	bool IsActionValid() const override;
	void WhenActionActived() override;
	void WhenActionAborted() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
public:
	UPROPERTY(SaveGame)
	int32 TargetPointIndex = INDEX_NONE;
	UPROPERTY(SaveGame)
	uint8 bMoveReverse : 1;
	UPROPERTY()
	UARPG_AD_CharacterBase* CurBehavior;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "移动者"))
	TSoftObjectPtr<ACharacterBase> Mover;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "路径"))
	TSoftObjectPtr<AARPG_NavPath> Path;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "沿路径移动行为配置"))
	TSubclassOf<UARPG_FollowPathMoveConfigBase> FollowPathMoveConfig;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	UPROPERTY(SaveGame, BlueprintReadWrite, meta = (DisplayName = "当无法到达"))
	FOnDispatchableActionFinishedEvent WhenCanNotReached;
private:
	void MoveToNavPoint(const FARPG_NavPathPoint& NavPoint, ACharacterBase* Character);
	void WhenMoveFinished(const FPathFollowingResult& Result);
};
