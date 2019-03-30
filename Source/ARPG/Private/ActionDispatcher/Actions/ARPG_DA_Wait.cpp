// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_Wait.h"
#include "GameFramework/Pawn.h"

bool UARPG_DA_Wait::IsActionValid() const
{
	return Pawn.Get() != nullptr;
}

void UARPG_DA_Wait::WhenActionActived()
{
	RegisterEntity(Pawn.Get());
}

void UARPG_DA_Wait::WhenActionDeactived()
{

}

void UARPG_DA_Wait::WhenActionFinished()
{
	UnregisterEntity(Pawn.Get());
}
