// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "ARPG_DA_MoveTo.generated.h"

struct FPathFollowingResult;
class ACharacterBase;

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UARPG_DA_MoveToBase : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TSet<AActor*> GetAllRegistableEntities() const override;
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
	TSoftObjectPtr<ACharacterBase> Mover;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "有效半径"))
	float AcceptRadius = 5.f;
};

UCLASS(meta = (DisplayName = "移动至目标"))
class ARPG_API UARPG_DA_MoveToActor : public UARPG_DA_MoveToBase
{
	GENERATED_BODY()
public:
	TSet<AActor*> GetAllRegistableEntities() const override;
	bool IsActionValid() const override;
	void WhenActionActived() override;
public:
	//目标
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "目标"))
	TSoftObjectPtr<AActor> Goal;

	//使用到达时朝向
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "使用到达时朝向"))
	uint8 bTurnTo : 1;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "转向目标", AllowEmpty = true))
	TSoftObjectPtr<AActor> TurnToActor;
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
	uint8 bTurnTo : 1;

	//到达时朝向
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "到达时朝向"))
	FRotator TurnToRotation;
};

UCLASS(meta = (DisplayName = "带领他人移动至目标"))
class ARPG_API UARPG_DA_MoveToActor_LeadOther : public UARPG_DA_MoveToActor
{
	GENERATED_BODY()
public:
	UARPG_DA_MoveToActor_LeadOther();

	TSet<AActor*> GetAllRegistableEntities() const override;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "他人"))
	TSoftObjectPtr<AActor> Follower;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "起始等待距离"))
	float WaitDistance = 1000.f;

	void WhenTick(float DeltaSeconds) override;
};

UCLASS(meta = (DisplayName = "带领他人移动至目的地"))
class ARPG_API UARPG_DA_MoveToLocation_LeadOther : public UARPG_DA_MoveToLocation
{
	GENERATED_BODY()
public:
	UARPG_DA_MoveToLocation_LeadOther();

	TSet<AActor*> GetAllRegistableEntities() const override;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "他人"))
	TSoftObjectPtr<AActor> Follower;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", DisplayName = "起始等待距离"))
	float WaitDistance = 1000.f;

	void WhenTick(float DeltaSeconds) override;
};
