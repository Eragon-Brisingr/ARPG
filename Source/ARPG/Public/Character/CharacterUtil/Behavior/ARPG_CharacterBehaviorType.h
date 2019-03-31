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

	//e.g.电梯的情况
	UPROPERTY(EditAnywhere, Category = "行为")
	uint8 bAttachToActor : 1;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "行为", Instanced)
	UARPG_CharacterBehaviorBase* Behavior = nullptr;
	
	UARPG_CharacterBehaviorBase* WorldPositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished) const;

	UARPG_CharacterBehaviorBase* RelativePositionExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, AActor* Owner) const;

	static void WhenBehaviorFinished(bool Succeed, class ACharacterBase* Character, FOnCharacterBehaviorFinished OnBehaviorFinished);
private:
	UARPG_CharacterBehaviorBase* ExecuteBehavior(class ACharacterBase* Character, const FOnCharacterBehaviorFinished& OnBehaviorFinished, const FVector& WorldLocation) const;
};
