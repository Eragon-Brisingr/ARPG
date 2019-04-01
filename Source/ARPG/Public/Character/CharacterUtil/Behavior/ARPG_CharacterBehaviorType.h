// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dispatcher/ARPG_AD_InteractableBase.h"
#include "ARPG_CharacterBehaviorType.generated.h"

class UARPG_CharacterBehaviorBase;


/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnCharacterBehaviorFinished, bool /*Succeed*/)
DECLARE_DELEGATE(FOnCharacterBehaviorAbortFinished);

USTRUCT(BlueprintType)
struct ARPG_API FBehaviorPositionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "行为")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bAttachToLocation : 1;

	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bAttachToRotation : 1;
};
