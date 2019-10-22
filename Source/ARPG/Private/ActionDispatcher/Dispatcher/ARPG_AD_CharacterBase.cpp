// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AD_CharacterBase.h"

UARPG_AD_CharacterBase::UARPG_AD_CharacterBase()
{
	bIsMainDispatcher = false;
}

void UARPG_AD_CharacterBase::WhenDeactived(bool IsFinsihedCompleted)
{
	Super::WhenDeactived(IsFinsihedCompleted);

	Reset();
}
