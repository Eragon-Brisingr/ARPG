// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_SneakSystemBase.h"
#include "CharacterBase.h"

class UWorld* UARPG_SneakSystemBase::GetWorld() const
{
	return Character->GetWorld();
}

float UARPG_SneakSystemBase::GetSightHideValue() const
{
	return IsSneaking() ? GetSightSneakValue() : GetSightNormalValue();
}

float UARPG_SneakSystemBase::GetHearHideValue() const
{
	return IsSneaking() ? GetHearSneakValue() : GetHearNormalValue();
}

float UARPG_SneakSystemBase::GetNoseHideValue() const
{
	return IsSneaking() ? GetNoseSneakValue() : GetNoseNormalValue();
}

bool UARPG_SneakSystemBase::IsSneaking() const
{
	return Character->IsSneaking();
}
