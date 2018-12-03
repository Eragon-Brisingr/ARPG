// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterBehaviorImpl.generated.h"

/**
 * 
 */

UCLASS(meta = (DisplayName = "行为_播放蒙太奇"))
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
	void WhenMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};

UCLASS(meta = (DisplayName = "行为_原地等待"))
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

UCLASS(meta = (DisplayName = "行为_播放蒙太奇[状态]"))
class ARPG_API UCBC_PlayStateMontage : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_PlayStateMontage();

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* StartMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* LoopMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* EndMontage;
};

UCLASS()
class ARPG_API UCB_PlayStateMontage : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer, const FVector& Location, const FRotator& Rotation) override;

	void AbortBehavior(class ACharacterBase* Executer) override;

	void WhenStartMontageEnd(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);

	void WhenLoopMontageEnd(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);

	void WhenEndMontageEnd(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);
public:
	const UCBC_PlayStateMontage* GetConfig() const;

	UAnimMontage* CurrentMontage;
};

