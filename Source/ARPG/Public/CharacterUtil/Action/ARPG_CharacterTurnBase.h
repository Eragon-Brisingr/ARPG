// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_CharacterActionType.h"
#include "ARPG_CharacterTurnBase.generated.h"

class UAnimMontage;
class ACharacterBase;

/**
 * 
 */
UCLASS(abstract, EditInlineNew, collapsecategories, Blueprintable, const)
class ARPG_API UARPG_CharacterTurnBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void TurnTo(ACharacterBase* Character, const FRotator& TargetWorldRotation, const FOnCharacterActionFinished& OnCharacterTurnFinished) const;
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|动作", meta = (DisplayName = "GetTurnMontage"))
	UAnimMontage* ReceiveGetTurnMontage(ACharacterBase* Character, const FRotator& TargetWorldRotation) const;

	void WhenMontageBlendOut(UAnimMontage* Montage, bool bInterrupted, FOnCharacterActionFinished OnCharacterTurnFinished);

	UFUNCTION(BlueprintCallable, Category = "角色|动作")
	UAnimMontage* GetTurnMontage(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180) const;
};

UCLASS(meta = (DisplayName = "转身_普通"))
class ARPG_API UARPG_CharacterTurnNormal : public UARPG_CharacterTurnBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "转身")
	UAnimMontage* TurnLeft90;
	UPROPERTY(EditAnywhere, Category = "转身")
	UAnimMontage* TurnRight90;
	UPROPERTY(EditAnywhere, Category = "转身")
	UAnimMontage* TurnLeft180;
	UPROPERTY(EditAnywhere, Category = "转身")
	UAnimMontage* TurnRight180;

	void TurnTo(ACharacterBase* Character, const FRotator& TargetWorldRotation, const FOnCharacterActionFinished& OnCharacterTurnFinished) const override;
};
