// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AlertSystemBase.h"
#include "CharacterBase.h"

class UWorld* UARPG_AlertSystemBase::GetWorld() const
{
	return Character->GetWorld();
}
