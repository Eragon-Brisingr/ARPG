// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Engine/EngineTypes.h>
#include "ReceiveDamageActionBase.generated.h"

/**
 * 
 */
UCLASS(const, Abstract, Blueprintable, hidedropdown)
class ARPG_API UReceiveDamageActionBase : public UObject
{
	GENERATED_BODY()
public:
	/** 返回值代表是否播放了受击动作 */
	UFUNCTION(BlueprintNativeEvent, Category = "受击行为")
	bool PlayReceiveDamageAction(const FVector& HitFromDirection, class ACharacterBase* ReceivedDamageCharacter, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const;
	virtual bool PlayReceiveDamageAction_Implementation(const FVector& HitFromDirection, class ACharacterBase* ReceivedDamageCharacter, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const { return false; }
};
