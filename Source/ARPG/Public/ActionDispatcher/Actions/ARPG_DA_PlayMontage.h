// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/XD_DispatchableActionBase.h"
#include "ARPG_DA_PlayMontage.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_DA_PlayMontage : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	bool CanActiveAction() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;

public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<APawn> Pawn;

	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UAnimMontage* MontageToPlay;

protected:
	UPROPERTY(SaveGame, meta = (DisplayName = "µ±²¥·Å½áÊø"))
	FOnDispatchableActionFinishedEvent WhenPlayFinished;

	void WhenMontageBlendingOutStart(UAnimMontage* Montage, bool bInterrupted);
};
