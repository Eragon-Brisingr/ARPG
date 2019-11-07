// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_MoneyCoreBase.generated.h"

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "金钱"))
class ARPG_API UARPG_MoneyCoreBase : public UARPG_ItemCoreBase
{
	GENERATED_BODY()
public:	
	FText GetItemTypeDesc() const override;
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
};
