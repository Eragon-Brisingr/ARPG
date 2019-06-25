// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterActionType.h"
#include "Engine/DataAsset.h"
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
	void TurnTo(ACharacterBase* Executer, const FRotator& TargetWorldRotation, const FOnCharacterBehaviorFinished& OnCharacterTurnFinished);

	void AbortTurnTo(ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& OnCharacterBehaviorAbortFinished);

	virtual bool CanTurnTo(const ACharacterBase* Executer) const { return false; }
protected:
	UPROPERTY(EditAnywhere, Category = "行为")
	FRotator TurnToRotation;
};

UCLASS(abstract, const, Blueprintable)
class ARPG_API UCA_TurnConfigBase : public UDataAsset
{
	GENERATED_BODY()
public:
	virtual UCA_CharacterTurnBase* CreateInstance(UObject* Outer) const { return nullptr; }
};

UCLASS(abstract)
class ARPG_API UCA_TurnMontageConfigBase : public UCA_TurnConfigBase
{
	GENERATED_BODY()
public:
	UCA_CharacterTurnBase* CreateInstance(UObject* Outer) const override;

	virtual UAnimMontage* GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation) const { return ReceiveGetTurnMontage(Executer, TargetWorldRotation); }
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|动作", meta = (DisplayName = "GetTurnMontage"))
	UAnimMontage* ReceiveGetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation) const;
	
	UFUNCTION(BlueprintCallable, Category = "角色|动作")
	UAnimMontage* GetTurnMontageFourDirection(const FRotator& CurrentWorldRotation, const FRotator& TargetWorldRotation, UAnimMontage* TurnLeft90, UAnimMontage* TurnRight90, UAnimMontage* TurnLeft180, UAnimMontage* TurnRight180) const;
};

UCLASS(meta = (DisplayName = "转身配置_普通"))
class ARPG_API UCA_TurnMontageConfigNormal : public UCA_TurnMontageConfigBase
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

	UAnimMontage* GetTurnMontage(ACharacterBase* Executer, const FRotator& TargetWorldRotation) const override;
};

UCLASS(meta = (DisplayName = "转身_播放蒙太奇"))
class ARPG_API UCA_TurnByMontage : public UCA_CharacterTurnBase
{
	GENERATED_BODY()
public:
	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;
	void WhenBehaviorAborted(ACharacterBase* Executer) override;
	bool CanTurnTo(const ACharacterBase* Executer) const override;

	UPROPERTY(EditAnywhere, Category = "配置")
	const UCA_TurnMontageConfigBase* TurnMontageConfig;

	UPROPERTY()
	UAnimMontage* CurrentTurnMontage;
};
