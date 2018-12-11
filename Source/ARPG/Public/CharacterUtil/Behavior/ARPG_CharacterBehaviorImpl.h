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
	void ExecuteBehavior(class ACharacterBase* Executer) override;

	void AbortBehavior(class ACharacterBase* Executer) override;

	const class UCBC_PlayMontage* GetConfig() const;
public:
	void WhenMontageBlendingOutStart(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);
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
	void ExecuteBehavior(class ACharacterBase* Executer) override;

	void AbortBehavior(class ACharacterBase* Executer) override;
public:
	const UCBC_Wait* GetConfig() const;

	FTimerHandle TimerHandle;
};

UCLASS(abstract)
class ARPG_API UCBC_PlayStateMontageSimpleBase : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_PlayStateMontageSimpleBase();

	virtual UAnimMontage* GetStartMontage() const { return nullptr; }

	virtual UAnimMontage* GetLoopMontage() const { return nullptr; }

	virtual UAnimMontage* GetEndMontage() const { return nullptr; }
};

UCLASS(meta = (DisplayName = "行为_播放蒙太奇[状态]简单"))
class ARPG_API UCBC_PlayStateMontageSimple : public UCBC_PlayStateMontageSimpleBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* StartMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* LoopMontage;

	UPROPERTY(EditAnywhere, Category = "行为")
	UAnimMontage* EndMontage;

public:
	UAnimMontage* GetStartMontage() const override { return StartMontage; }

	UAnimMontage* GetLoopMontage() const override { return LoopMontage; }

	UAnimMontage* GetEndMontage() const override { return EndMontage; }
};

UCLASS(meta = (DisplayName = "行为_播放蒙太奇[状态]简单随机"))
class ARPG_API UCBC_PlayStateMontageSimpleRandom : public UCBC_PlayStateMontageSimpleBase
{
	GENERATED_BODY()
public:
	UCBC_PlayStateMontageSimpleRandom();

	UAnimMontage* GetStartMontage() const override;

	UAnimMontage* GetLoopMontage() const override;

	UAnimMontage* GetEndMontage() const override;
public:
	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> StartMontages;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> LoopMontages;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> EndMontages;
};

UCLASS(abstract)
class ARPG_API UCB_PlayStateMontageBase : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer) override;

	void AbortBehavior(class ACharacterBase* Executer) override;

	void WhenStartMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);

	void WhenLoopMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);

	void WhenEndMontageBlendingOutStarted(UAnimMontage* Montage, bool bInterrupted, class ACharacterBase* Executer);
public:
	UAnimMontage* CurrentMontage;

	virtual UAnimMontage* GetStartMontage() const { return nullptr; }

	virtual UAnimMontage* GetLoopMontage() const { return nullptr; }

	virtual UAnimMontage* GetEndMontage() const { return nullptr; }
};

UCLASS()
class ARPG_API UCB_PlayStateMontageSimple : public UCB_PlayStateMontageBase
{
	GENERATED_BODY()
public:
	const UCBC_PlayStateMontageSimpleBase* GetConfig() const;

	virtual UAnimMontage* GetStartMontage() const;

	virtual UAnimMontage* GetLoopMontage() const;

	virtual UAnimMontage* GetEndMontage() const;
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
class ARPG_API UCBC_PlayStateMontageStandard : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_PlayStateMontageStandard();

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<UAnimMontage*> StartMontages;

	UPROPERTY(EditAnywhere, Category = "行为")
	TArray<FPlayStateMontageStandardConfig> LoopConfig;
};

UCLASS()
class ARPG_API UCB_PlayStateMontageStandard : public UCB_PlayStateMontageBase
{
	GENERATED_BODY()
public:
	const UCBC_PlayStateMontageStandard* GetConfig() const;

	UAnimMontage* GetStartMontage() const override;

	UAnimMontage* GetLoopMontage() const override;

	UAnimMontage* GetEndMontage() const override;

	mutable int32 CurLoopRandomIndex;
};

UCLASS(meta = (DisplayName = "行为_转向"))
class ARPG_API UCBC_TurnTo : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_TurnTo();

	UPROPERTY(EditAnywhere, Category = "行为")
	FRotator TargetWorldRotation;
};

UCLASS()
class ARPG_API UCB_TurnTo : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer) override;
};

//e.g. 人类收回武器
UCLASS(meta = (DisplayName = "行为_进入放松状态"))
class ARPG_API UCBC_EnterReleaseState : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_EnterReleaseState();
};

UCLASS()
class ARPG_API UCB_EnterReleaseState : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer) override;
};
