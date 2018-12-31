// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterActionType.h"
#include "ARPG_CharacterTurnBase.generated.h"

class UAnimMontage;
class ACharacterBase;

/**
 * 
 */
UCLASS(abstract, EditInlineNew, collapsecategories, Blueprintable)
class ARPG_API UCA_CharacterTurnBase : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	bool TurnTo(ACharacterBase* Executer, const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterTurnFinished);

	void AbortTurnTo(ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& OnCharacterBehaviorAbortFinished);

	virtual UAnimMontage* GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation) { return ReceiveGetTurnMontage(Executer, TargetWorldRotation); }
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|动作", meta = (DisplayName = "GetTurnMontage"))
	UAnimMontage* ReceiveGetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation);

	void WhenMontageBlendOutStart(UAnimMontage* Montage, bool bInterrupted);
protected:
	UFUNCTION(BlueprintCallable, Category = "角色|动作")
	UAnimMontage* GetTurnMontageFourDirection(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180);

	UPROPERTY()
	UAnimMontage* CurrentTurnMontage;
public:
	void AbortBehavior(ACharacterBase* Executer) override;
};

UCLASS(meta = (DisplayName = "转身_普通"))
class ARPG_API UCA_TurnNormal : public UCA_CharacterTurnBase
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

	UAnimMontage* GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation) override;
};
