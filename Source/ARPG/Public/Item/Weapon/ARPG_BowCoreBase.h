// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ARPG_WeaponCoreBase.h"
#include "ARPG_BowCoreBase.generated.h"

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "弓"))
class ARPG_API UARPG_BowCoreBase : public UARPG_WeaponCoreBase
{
	GENERATED_BODY()
	
public:
	FText GetItemTypeDesc_Implementation() const override;
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
};
