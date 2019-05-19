// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_CharacterMovementComponent.h"
#include "ARPG_MovementComponent.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_MovementComponent : public UXD_CharacterMovementComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	ACharacterBase* ARPG_Character;

	virtual void OnRegister() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMovementModeChanged, UARPG_MovementComponent*, MovementComponent, EMovementMode, PreviousMovementMode, uint8, PreviousCustomMode);
	UPROPERTY(BlueprintAssignable)
	FOnMovementModeChanged OnARPGMovementModeChanged;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	
};
