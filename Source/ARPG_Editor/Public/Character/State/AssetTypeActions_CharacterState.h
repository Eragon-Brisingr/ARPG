// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class ARPG_EDITOR_API FAssetTypeActions_CharacterState : public FAssetTypeActions_Base
{
public:
	using Super = FAssetTypeActions_Base;

 	FText GetName() const override;
 	UClass* GetSupportedClass() const override;
 	FColor GetTypeColor() const override;
 	uint32 GetCategories() override;
	FText GetDisplayNameFromAssetData(const FAssetData& AssetData) const override;
};
