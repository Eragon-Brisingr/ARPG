// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_ActionDispatcherBase.h"
#include "ARPG_AD_CharacterBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS(abstract, editinlinenew, collapseCategories, meta = (DisplayName = "角色调度器"))
class ARPG_API UARPG_AD_CharacterBase : public UARPG_ActionDispatcherBase
{
	GENERATED_BODY()
public:
	UARPG_AD_CharacterBase();

	UPROPERTY(BlueprintReadOnly, Category = "行为|交互", meta = (ExposeOnSpawn = true))
	TSoftObjectPtr<ACharacterBase> Character;
};
