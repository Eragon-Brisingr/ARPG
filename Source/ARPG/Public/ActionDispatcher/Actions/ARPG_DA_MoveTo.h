// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "ARPG_DA_MoveTo.generated.h"

struct FPathFollowingResult;

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UARPG_DA_MoveToBase : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	bool IsActionValid() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
protected:
	UPROPERTY(SaveGame, meta = (DisplayName = "当到达了"))
	FOnDispatchableActionFinishedEvent WhenReached;

	UPROPERTY(SaveGame, meta = (DisplayName = "当无法到达"))
	FOnDispatchableActionFinishedEvent WhenCanNotReached;

	void WhenRequestFinished(const FPathFollowingResult& Result);

	bool IsExistValidPath(const FVector& Start, const FVector& End) const;
public:
	//移动者
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "移动者"))
	TSoftObjectPtr<APawn> Mover;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "有效半径"))
	float AcceptRadius = 5.f;
};

UCLASS(meta = (DisplayName = "移动至目标"))
class ARPG_API UARPG_DA_MoveToActor : public UARPG_DA_MoveToBase
{
	GENERATED_BODY()
public:
	bool IsActionValid() const override;
	void WhenActionActived() override;
public:
	//目标
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "目标"))
	TSoftObjectPtr<AActor> Goal;
};

UCLASS(meta = (DisplayName = "移动至目的地"))
class ARPG_API UARPG_DA_MoveToLocation : public UARPG_DA_MoveToBase
{
	GENERATED_BODY()
public:
	bool IsActionValid() const override;
	void WhenActionActived() override;
public:
	//目的地
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "目的地"))
	FVector Destination;

	//使用到达时朝向
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "使用到达时朝向"))
	uint8 bUseRotation : 1;

	//到达时朝向
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "到达时朝向"))
	FRotator Rotation;
};

