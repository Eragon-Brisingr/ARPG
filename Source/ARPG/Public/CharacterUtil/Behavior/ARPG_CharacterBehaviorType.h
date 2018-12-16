// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorType.generated.h"

class UARPG_CharacterBehaviorConfigBase;
class UARPG_CharacterBehaviorConfigurable;

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnCharacterBehaviorFinished, bool /*Succeed*/)
DECLARE_DELEGATE(FOnCharacterBehaviorAbortFinished);

USTRUCT(BlueprintType)
struct ARPG_API FBehaviorWithPosition
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

	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bEnableHandIK : 1;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "行为", Instanced)
 	UARPG_CharacterBehaviorConfigBase* Behavior = nullptr;
	
	UARPG_CharacterBehaviorConfigurable* WorldPositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished) const;

	UARPG_CharacterBehaviorConfigurable* RelativePositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FTransform& Transform) const;

	static void WhenBehaviorFinished(bool Succeed, class ACharacterBase* Character, FOnCharacterBehaviorFinished OnBehaviorFinished);
private:
	UARPG_CharacterBehaviorConfigurable* ExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FVector& WorldLocation) const;
};
