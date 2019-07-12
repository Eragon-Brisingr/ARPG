// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Abstract/ARPG_ItemCoreBase.h"
#include "ARPG_WeaponCoreBase.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "武器核心"))
class ARPG_API UARPG_WeaponCoreBase : public UARPG_ItemCoreBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "武器", meta = (DisplayName = "等级"))
	int32 Level;
	
	
	
};
