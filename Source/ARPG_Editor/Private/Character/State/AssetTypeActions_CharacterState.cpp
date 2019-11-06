// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_CharacterState.h"
#include "AssetTypeCategories.h"
#include "ARPG_CharacterStateBlueprint.h"
#include "ARPG_CharacterStateBase.h"

#define LOCTEXT_NAMESPACE "ARPG_CharacterState"

FText FAssetTypeActions_CharacterState::GetName() const
{
	return LOCTEXT("角色状态", "角色状态");
}

UClass* FAssetTypeActions_CharacterState::GetSupportedClass() const
{
	return UARPG_CharacterStateBlueprint::StaticClass();
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
	FString NativeParentClassPath = AssetData.GetTagValueRef<FString>(FBlueprintTags::NativeParentClassPath);
	UObject* Outer = nullptr;
	ResolveName(Outer, NativeParentClassPath, false, false);
	UClass* ParentClass = FindObject<UClass>(ANY_PACKAGE, *NativeParentClassPath);
	if (ParentClass->IsChildOf(UARPG_CharacterState_AccumulationBase::StaticClass()))
	{
		return LOCTEXT("积累状态", "积累状态");
	}
	else if (ParentClass->IsChildOf(UARPG_CharacterState_BuffBase::StaticClass()))
	{
		return LOCTEXT("BUFF", "BUFF");
	}
	return Super::GetDisplayNameFromAssetData(AssetData);
}

#undef LOCTEXT_NAMESPACE
