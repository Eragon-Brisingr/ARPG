// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ARPG_CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_CharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UFUNCTION()
	void OnMovementModeChanged(class UARPG_MovementComponent* MovementComponent, EMovementMode PreviousMovementMode, uint8 PreviousCustomMode);
};
