// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EnterReleaseStateBase.h"

void UCA_EnterReleaseStateBase::EnterReleaseState(ACharacterBase* Executer, const FOnCharacterBehaviorFinished& OnCharacterBehaviorFinished)
{
	ExecuteBehavior(Executer, OnCharacterBehaviorFinished);
}
