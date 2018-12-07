// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorBase.h"
#include "CharacterBase.h"
#include "XD_ObjectFunctionLibrary.h"
#include "ARPG_AI_Log.h"

UWorld* UARPG_CharacterBehaviorBase::GetWorld() const
{
	return Character->GetWorld();
}

void UARPG_CharacterBehaviorBase::FinishExecute(bool Succeed)
{
	OnBehaviorFinished.ExecuteIfBound(Succeed);
	AI_V_Display_Log(Character, "行为[%s]结束，结果为", *UXD_ObjectFunctionLibrary::GetObjectClassName(this), Succeed ? TEXT("成功") : TEXT("失败"));
}

void UARPG_CharacterBehaviorBase::FinishAbort()
{
	OnBehaviorAbortFinished.ExecuteIfBound();
	AI_V_Display_Log(Character, "行为中止状态[%s]结束", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
}

void UARPG_CharacterBehaviorConfigBase::ExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished) const
{
	if (BehaviorType)
	{
		UARPG_CharacterBehaviorBase*& Behavior = BehaviorMap.FindOrAdd(Character);
		if (Behavior == nullptr)
		{
			Behavior = NewObject<UARPG_CharacterBehaviorBase>(const_cast<UARPG_CharacterBehaviorConfigBase*>(this), BehaviorType);
			Behavior->Character = Character;
			Behavior->Config = this;
		}
		Behavior->OnBehaviorFinished = OnBehaviorFinished;
		Behavior->ExecuteBehavior(Character);

		AI_V_Display_Log(Character, "执行行为[%s]", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
	}
	else
	{
		AI_V_Error_Log(Character, "行为配置[%s]中未填写行为类型", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
		OnBehaviorFinished.ExecuteIfBound(false);
	}
}

void UARPG_CharacterBehaviorConfigBase::AbortBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorAbortFinished& OnBehaviorAbortFinished)
{
	if (UARPG_CharacterBehaviorBase** P_Behavior = BehaviorMap.Find(Character))
	{
		UARPG_CharacterBehaviorBase* Behavior = *P_Behavior;

		Behavior->OnBehaviorAbortFinished = OnBehaviorAbortFinished;
		Behavior->AbortBehavior(Character);
		AI_V_Display_Log(Character, "行为[%s]中断", *UXD_ObjectFunctionLibrary::GetObjectClassName(this));
	}
}

FString UARPG_CharacterBehaviorConfigBase::GetDescribe() const
{
	FString Desc = UXD_ObjectFunctionLibrary::GetObjectClassName(this) + TEXT("\n");
	Desc += UXD_ObjectFunctionLibrary::GetObjectPropertysDesc(this, UARPG_CharacterBehaviorConfigBase::StaticClass());
	return Desc;
}
