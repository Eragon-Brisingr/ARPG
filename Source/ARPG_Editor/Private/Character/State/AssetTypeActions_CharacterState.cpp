// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_CharacterState.h"
#include "AssetTypeCategories.h"
#include "ARPG_CharacterStateBase.h"

#define LOCTEXT_NAMESPACE "ARPG_CharacterState"

FText FAssetTypeActions_CharacterState::GetName() const
{
	return LOCTEXT("角色状态", "角色状态");
}

UClass* FAssetTypeActions_CharacterState::GetSupportedClass() const
{
	return UARPG_CharacterStateBase::StaticClass();
}

FColor FAssetTypeActions_CharacterState::GetTypeColor() const
{
	return FColor::Purple;
}

uint32 FAssetTypeActions_CharacterState::GetCategories()
{
	return EAssetTypeCategories::Gameplay;
}

FText FAssetTypeActions_CharacterState::GetDisplayNameFromAssetData(const FAssetData& AssetData) const
{
	if (AssetData.GetClass()->IsChildOf<UARPG_CharacterState_AccumulationBase>())
	{
		return LOCTEXT("积累状态", "积累状态");
	}
	else if (AssetData.GetClass()->IsChildOf<UARPG_CharacterState_AccumulationBase>())
	{
		return LOCTEXT("BUFF", "BUFF");
	}
	return Super::GetDisplayNameFromAssetData(AssetData);
}

#undef LOCTEXT_NAMESPACE
