// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterBehaviorImpl.generated.h"

/**
 * 
 */

UCLASS(meta = (DisplayName = "播放蒙太奇"))
class ARPG_API UCBC_PlayMontage : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_PlayMontage();

	UPROPERTY(EditAnywhere, Category = "角色行为")
	class UAnimMontage* Montage;
};

UCLASS()
class ARPG_API UCB_PlayMontage : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation) override;

	void AbortBehavior(class ACharacterBase* Executer) override;

	const class UCBC_PlayMontage* GetConfig() const;
public:
	FTimerHandle TimerHandle;

	void WhenMontageEnd();
};

UCLASS(meta = (DisplayName = "原地等待"))
class ARPG_API UCBC_Wait : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_Wait();

	UPROPERTY(EditAnywhere, Category = "角色行为")
	float WaitTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "角色行为")
	float RandomRange = 3.f;
};

UCLASS()
class ARPG_API UCB_Wait : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation) override;

	void AbortBehavior(class ACharacterBase* Executer) override;
public:
	const UCBC_Wait* GetConfig() const;

	FTimerHandle TimerHandle;
};

