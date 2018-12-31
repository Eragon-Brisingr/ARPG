// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterBehaviorContainer.generated.h"

class ACharacterBase;

/**
 * 
 */
USTRUCT()
struct FRandomSelectBehaviorElement
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "容器")
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, Category = "容器", Instanced)
	UARPG_CharacterBehaviorConfigBase* Behavior;
};

UCLASS(meta = (DisplayName = "行为容器_随机选择"))
class ARPG_API UCBC_RandomSelect : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_RandomSelect();

	UPROPERTY(EditAnywhere, Category = "容器")
	TArray<FRandomSelectBehaviorElement> RandomBehaviors;
};

UCLASS()
class ARPG_API UCB_RandomSelect : public UARPG_CharacterBehaviorConfigurable
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer) override;

	void AbortBehavior(ACharacterBase* Executer) override;
public:
	const UCBC_RandomSelect* GetConfig() const;

	UARPG_CharacterBehaviorConfigBase* CurrentBehavior;
};

UCLASS(meta = (DisplayName = "行为容器_顺序执行"))
class ARPG_API UCBC_Sequence : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_Sequence();

	UPROPERTY(EditAnywhere, Category = "容器", Instanced)
	TArray<UARPG_CharacterBehaviorConfigBase*> Behaviors;
};

UCLASS()
class ARPG_API UCB_Sequence : public UARPG_CharacterBehaviorConfigurable
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(class ACharacterBase* Executer) override;

	void AbortBehavior(ACharacterBase* Executer) override;

	const UCBC_Sequence* GetConfig() const;

	void WhenElementBehaviorFinished(bool Succeed, ACharacterBase* Executer);

	void WhenElementBehaviorAbortFinished();

	void ExecuteElement(ACharacterBase* Executer);

public:
	int32 ExecuteIndex;
};

UCLASS(meta = (DisplayName = "行为容器_状态行为"))
class ARPG_API UCBC_StateBehavior : public UARPG_CharacterBehaviorConfigBase
{
	GENERATED_BODY()
public:
	UCBC_StateBehavior();

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_CharacterBehaviorConfigBase* StartBehavior;

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_CharacterBehaviorConfigBase* LoopBehavior;

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_CharacterBehaviorConfigBase* EndBehavior;
};

UCLASS()
class ARPG_API UCB_StateBehavior : public UARPG_CharacterBehaviorConfigurable
{
	GENERATED_BODY()
public:
	void ExecuteBehavior(ACharacterBase* Executer) override;

	void AbortBehavior(ACharacterBase* Executer) override;
private:
	UPROPERTY()
	UARPG_CharacterBehaviorConfigBase* CurrentBehavior;

	const UCBC_StateBehavior* GetConfig() const;

	void WhenStartBehaviorEnd(bool Succeed, ACharacterBase* Executer);

	void WhenLoopBehaviorEnd(bool Succeed, ACharacterBase* Executer);

	void WhenEndBehaviorEnd(bool Succeed, ACharacterBase* Executer);

	void WhenAbortBehaviorEnd(ACharacterBase* Executer);
};
