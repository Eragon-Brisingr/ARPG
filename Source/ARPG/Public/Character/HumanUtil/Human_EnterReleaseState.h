// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_EnterReleaseStateBase.h"
#include "Human_EnterReleaseState.generated.h"

class UAnimMontage;
class ACharacterBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "人类_进入松懈状态"))
class ARPG_API UHuman_EnterReleaseState : public UCA_EnterReleaseStateBase
{
	GENERATED_BODY()
public:
	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;

	void WhenBehaviorAborted(class ACharacterBase* Executer) override;

	void WhenMontageBlendOutStart(UAnimMontage* Montage, bool bInterrupted);

	UAnimMontage* CurMontage;
};
