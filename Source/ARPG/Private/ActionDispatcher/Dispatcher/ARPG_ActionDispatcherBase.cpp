// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ActionDispatcherBase.h"

#include "ARPG_DA_RoleSelection.h"
#include "ARPG_DA_PlayMontage.h"
#include "ARPG_DA_Dialogue.h"

UARPG_ActionDispatcherBase::UARPG_ActionDispatcherBase()
	:bInteractable(true)
{

}

namespace ActionCompatibleMapUtils
{
	// 这边的顺序必须与ActionCompatibleMap一致
	const TArray<TSubclassOf<UXD_DispatchableActionBase>>& GetActionIdxs()
	{
		static TArray<TSubclassOf<UXD_DispatchableActionBase>> ActionIdxs = { 
			UARPG_DA_RoleSelectionBase::StaticClass(), 
			UARPG_DA_PlayMontage::StaticClass(), 
			UARPG_DA_SpeakOneSentence::StaticClass() 
		};
		return ActionIdxs;
	}
	static TArray<TArray<bool>> ActionCompatibleMap =
	{								/*RoleSelectionBase*/	/*PlayMontage*/		/*SpeakOneSentence*/
		/*RoleSelectionBase*/		{false,					true,				true},
		/*PlayMontage*/				{						false,				true},
		/*SpeakOneSentence*/		{											false},
	};
	int32 GetActionIdx(const UXD_DispatchableActionBase* Action)
	{
		for (TSubclassOf<UXD_DispatchableActionBase> Class = Action->GetClass(); Class->IsChildOf<UXD_DispatchableActionBase>(); Class = Class->GetSuperClass())
		{
			int32 Idx = GetActionIdxs().IndexOfByKey(Class);
			if (Idx != INDEX_NONE)
			{
				return Idx;
			}
		}
		return INDEX_NONE;
	};
	bool ActionIsBothCompatible(UXD_DispatchableActionBase* LHS, UXD_DispatchableActionBase* RHS)
	{
		check(ActionCompatibleMap.Num() == GetActionIdxs().Num());

		int32 LHS_Idx = GetActionIdx(LHS);
		int32 RHS_Idx = GetActionIdx(RHS);
		if (LHS_Idx != INDEX_NONE && RHS_Idx != INDEX_NONE)
		{
			return ActionCompatibleMap[FMath::Min(LHS_Idx, RHS_Idx)][ActionCompatibleMap.Num() - FMath::Max(LHS_Idx, RHS_Idx)];
		}
		return false;
	}
}

bool UARPG_ActionDispatcherBase::ActionIsBothCompatible(UXD_DispatchableActionBase* LHS, UXD_DispatchableActionBase* RHS) const
{
	return ActionCompatibleMapUtils::ActionIsBothCompatible(LHS, RHS);
}
