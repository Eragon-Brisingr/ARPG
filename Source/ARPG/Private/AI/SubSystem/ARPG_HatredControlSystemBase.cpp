// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_HatredControlSystemBase.h"
#include "CharacterBase.h"


UWorld* UARPG_HatredControlSystemBase::GetWorld() const
{
	return Character->GetWorld();
}
