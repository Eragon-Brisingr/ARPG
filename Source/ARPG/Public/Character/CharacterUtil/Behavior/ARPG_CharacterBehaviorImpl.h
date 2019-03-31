// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterBehaviorImpl.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "行为_播放蒙太奇"))
class ARPG_API UCB_PlayMontage : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "角色行为")
	class UAnimMontage* MontageToPlay;

	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;

	void WhenBehaviorAborted(class ACharacterBase* Executer) override;
public:
	void WhenMontageBlendingOutStart(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);
};

UCLASS(meta = (DisplayName = "行为_原地等待"))
class ARPG_API UCB_Wait : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "角色行为")
	float WaitTime = 5.f;

	UPROPERTY(EditAnywhere, Category = "角色行为")
	float RandomRange = 3.f;

	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;

	void WhenBehaviorAborted(class ACharacterBase* Executer) override;
public:
	FTimerHandle TimerHandle;
};

UCLASS(abstract)
class ARPG_API UCB_PlayStateMontageBase : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;

	void WhenBehaviorAborted(class ACharacterBase* Executer) override;

	void WhenStartMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);

	void WhenLoopMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);

	void WhenEndMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);
public:
	UAnimMontage* CurrentMontage;

	virtual UAnimMontage* GetStartMontage() const { return nullptr; }

	virtual UAnimMontage* GetLoopMontage() const { return nullptr; }

	virtual UAnimMontage* GetEndMontage() const { return nullptr; }
};

UCLASS(meta = (DisplayName = "行为_播放蒙太奇[状态]简单"))
class ARPG_API UCB_PlayStateMontageSimple : public UCB_PlayStateMontageBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* StartMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* LoopMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* EndMontage;

	UAnimMontage* GetStartMontage() const override;

	UAnimMontage* GetLoopMontage() const override;

	UAnimMontage* GetEndMontage() const override;
};

UCLASS(meta = (DisplayName = "行为_播放蒙太奇[状态]简单随机"))
class ARPG_API UCB_PlayStateMontageSimpleRandom : public UCB_PlayStateMontageBase
{
	GENERATED_BODY()
public:
	UCB_PlayStateMontageSimpleRandom();

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> StartMontages;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> LoopMontages;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> EndMontages;

	UAnimMontage* GetStartMontage() const override;

	UAnimMontage* GetLoopMontage() const override;

	UAnimMontage* GetEndMontage() const override;
};

USTRUCT()
struct FPlayStateMontageStandardConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* LoopMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> EndMontages{nullptr};
};

UCLASS(meta = (DisplayName = "行为_播放蒙太奇[状态]标准"))
class ARPG_API UCB_PlayStateMontageStandard : public UCB_PlayStateMontageBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> StartMontages;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<FPlayStateMontageStandardConfig> LoopConfig;

	UAnimMontage* GetStartMontage() const override;

	UAnimMontage* GetLoopMontage() const override;

	UAnimMontage* GetEndMontage() const override;

	mutable int32 CurLoopRandomIndex;
};

UCLASS(meta = (DisplayName = "行为_转向"))
class ARPG_API UCB_TurnTo : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	FRotator TargetWorldRotation;

	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;
};

UCLASS(meta = (DisplayName = "行为_进入放松状态"))
class ARPG_API UCB_EnterReleaseState : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;
};
