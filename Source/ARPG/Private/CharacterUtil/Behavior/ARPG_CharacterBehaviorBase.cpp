// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterBehaviorBase.h"
#include "CharacterBase.h"
#include "XD_ObjectFunctionLibrary.h"

UWorld* UARPG_CharacterBehaviorBase::GetWorld() const
{
	return Character->GetWorld();
}

void UARPG_CharacterBehaviorBase::FinishBehavior()
{
	OnFinishBehavior.ExecuteIfBound();
}

void UARPG_CharacterBehaviorConfigBase::ExecuteBehavior(class ACharacterBase* Character, const FVector& Location, const FRotator& Rotation, const FSimpleDelegate& OnFinishBehavior) const
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
		Behavior->OnFinishBehavior = OnFinishBehavior;
		Behavior->ExecuteBehavior(Character, Location, Rotation);
	}
	else
	{
		OnFinishBehavior.ExecuteIfBound();
	}
}

void UARPG_CharacterBehaviorConfigBase::AbortBehavior(class ACharacterBase* Character)
{
	if (UARPG_CharacterBehaviorBase** P_Behavior = BehaviorMap.Find(Character))
	{
		(*P_Behavior)->AbortBehavior(Character);
	}
}

FString UARPG_CharacterBehaviorConfigBase::GetDescribe() const
{
	FString Desc = UXD_ObjectFunctionLibrary::GetObjectClassName(this) + TEXT("\n");
	Desc += UXD_ObjectFunctionLibrary::GetObjectPropertysDesc(this, UARPG_CharacterBehaviorConfigBase::StaticClass());
	return Desc;
}
