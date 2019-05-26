// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_NavAreaType.h"

UARPG_NavArea_JumpBase::UARPG_NavArea_JumpBase()
{
	FARPG_NavAreaFlagHelper::Set(AreaFlags, EARPG_NavAreaFlag::Jump);

	DefaultCost = 1.5f;
	FixedAreaEnteringCost = 2.f;

	DrawColor = FColor(0, 255, 255);
}
