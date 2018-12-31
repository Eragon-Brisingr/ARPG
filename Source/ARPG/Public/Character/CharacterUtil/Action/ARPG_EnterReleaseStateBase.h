// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_EnterReleaseStateBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ARPG_API UCA_EnterReleaseStateBase : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void EnterReleaseState(ACharacterBase* Executer, const FOnCharacterBehaviorFinished& OnCharacterBehaviorFinished);

	virtual void ExecuteEnterReleaseState(ACharacterBase* Executer){}
};
