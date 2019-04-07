// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionDispatcher/Dispatcher/ARPG_ActionDispatcherBase.h"
#include "ARPG_AD_CharacterInteract.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS(abstract, editinlinenew, collapseCategories, meta = (DisplayName = "角色交互调度器"))
class ARPG_API UARPG_AD_CharacterInteract : public UARPG_ActionDispatcherBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "交互")
	TSoftObjectPtr<ACharacterBase> Character;

	UPROPERTY(BlueprintReadOnly, Category = "交互")
	TSoftObjectPtr<ACharacterBase> InteractTarget;
};
