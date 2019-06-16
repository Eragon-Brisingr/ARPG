// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_DA_Wait.h"
#include "GameFramework/Pawn.h"
#include "ARPG_TimeManager.h"

TArray<AActor*> UARPG_DA_Wait::GetAllRegistableEntities() const
{
	return { Pawn.Get() };
}

bool UARPG_DA_Wait::IsActionValid() const
{
	return Pawn.IsValid();
}

void UARPG_DA_Wait::WhenActionActived()
{

}

void UARPG_DA_Wait::WhenActionDeactived()
{

}

void UARPG_DA_Wait::WhenActionFinished()
{

}

TArray<AActor*> UARPG_DA_WaitGameTime::GetAllRegistableEntities() const
{
	return { Pawn.Get() };
}

bool UARPG_DA_WaitGameTime::IsActionValid() const
{
	return Pawn.IsValid();
}

void UARPG_DA_WaitGameTime::WhenActionActived()
{
	UARPG_TimeManager* TimeManager = UARPG_TimeManager::GetARPG_TimeManager(this);
	APawn* Character = Pawn.Get();
	GameTimeHandle = TimeManager->AddNativeDelayTimeEvent(GameTimeSpan, FXD_GameTimeNativeDelegate::CreateWeakLambda(Character, [=]()
		{
			ExecuteEventAndFinishAction(WhenWaitFinished);
		}));
}

void UARPG_DA_WaitGameTime::WhenActionDeactived()
{
	UARPG_TimeManager* TimeManager = UARPG_TimeManager::GetARPG_TimeManager(this);
	TimeManager->RemoveNativeSpecialGameTimeEvent(GameTimeHandle);
}

void UARPG_DA_WaitGameTime::WhenActionFinished()
{

}
