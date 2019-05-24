// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_NavAreaType.h"

UARPG_NavArea_Jump::UARPG_NavArea_Jump()
{
	FARPG_NavAreaFlagHelper::Set(AreaFlags, EARPG_NavAreaFlag::Jump);
}
