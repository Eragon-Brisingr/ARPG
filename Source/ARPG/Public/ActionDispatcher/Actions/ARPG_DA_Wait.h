// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "ARPG_DA_Wait.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "等待"))
class ARPG_API UARPG_DA_Wait : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	bool CanActiveAction() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;

protected:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<APawn> Pawn;
};
