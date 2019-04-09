// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorBase.h"
#include "CharacterBase.h"
#include "XD_ObjectFunctionLibrary.h"
#include "ARPG_AI_Log.h"

UWorld* UARPG_CharacterBehaviorBase::GetWorld() const
{
	return Character->GetWorld();
}

void UARPG_CharacterBehaviorBase::ExecuteBehavior(class ACharacterBase* Executer, const FOnCharacterBehaviorFinished& WhenBehaviorFinished)
{
	check(bIsExecuting == false);

	bIsExecuting = true;
	Character = Executer;
	OnBehaviorFinished = WhenBehaviorFinished;
	AI_V_Display_Log(Character, "执行行为[%s]", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
	WhenBehaviorExecuted(Executer);
}

void UARPG_CharacterBehaviorBase::AbortBehavior(class ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& WhenBehaviorAbortFinished)
{
	check(bIsExecuting == true);

	bIsExecuting = false;
	OnBehaviorAbortFinished = WhenBehaviorAbortFinished;
	WhenBehaviorAborted(Executer);
	AI_V_Display_Log(Executer, "行为[%s]中断", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
}

void UARPG_CharacterBehaviorBase::FinishExecute(bool Succeed)
{
	check(bIsExecuting == true);

	bIsExecuting = false;
	OnBehaviorFinished.ExecuteIfBound(Succeed);
	OnBehaviorFinished.Unbind();
	AI_V_Display_Log(Character, "行为[%s]结束，结果为", *UXD_ObjectFunctionLibrary::GetObjectClassName(this), Succeed ? TEXT("成功") : TEXT("失败"));
}

void UARPG_CharacterBehaviorBase::FinishAbort()
{
	OnBehaviorAbortFinished.ExecuteIfBound();
	OnBehaviorAbortFinished.Unbind();
	AI_V_Display_Log(Character, "行为中止状态[%s]结束", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
}

FString UARPG_CharacterBehaviorBase::GetDescribe() const
{
	FString Desc = UXD_ObjectFunctionLibrary::GetObjectClassName(this) + TEXT("\n");
	Desc += UXD_ObjectFunctionLibrary::GetObjectPropertysDesc(this, UARPG_CharacterBehaviorBase::StaticClass());
	return Desc;
}
