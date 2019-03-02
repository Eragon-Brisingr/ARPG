// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "ARPG_DA_MoveTo.generated.h"

struct FPathFollowingResult;

/**
 * 
 */
UCLASS(meta = (DisplayName = "移动至"))
class ARPG_API UARPG_DA_MoveTo : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;

protected:
	UPROPERTY(SaveGame)
	FDispatchableActionFinishedEvent WhenReached;

	UPROPERTY(SaveGame)
	FDispatchableActionFinishedEvent WhenCanNotReached;

	void WhenRequestFinished(const FPathFollowingResult& Result);
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<APawn> Pawn;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FVector Location;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<AActor> Goal;
};
