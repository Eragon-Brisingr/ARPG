// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_CharacterBehaviorContainer.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UCB_ContainerBase : public UARPG_CharacterBehaviorBase
{
	GENERATED_BODY()
};

USTRUCT()
struct FRandomSelectBehaviorElement
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "容器")
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, Category = "容器", Instanced)
	UARPG_CharacterBehaviorBase* Behavior;
};

UCLASS(meta = (DisplayName = "行为容器_随机选择"))
class ARPG_API UCB_RandomSelect : public UCB_ContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "容器")
	TArray<FRandomSelectBehaviorElement> RandomBehaviors;

	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;

	void WhenBehaviorAborted(ACharacterBase* Executer) override;
public:
	UARPG_CharacterBehaviorBase* CurrentBehavior;
};

UCLASS(meta = (DisplayName = "行为容器_顺序执行"))
class ARPG_API UCB_Sequence : public UCB_ContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "容器", Instanced)
	TArray<UARPG_CharacterBehaviorBase*> Behaviors;

	void WhenBehaviorExecuted(class ACharacterBase* Executer) override;

	void WhenBehaviorAborted(ACharacterBase* Executer) override;

	void WhenElementBehaviorFinished(bool Succeed, ACharacterBase* Executer);

	void WhenElementBehaviorAbortFinished();

	void ExecuteElement(ACharacterBase* Executer);

public:
	int32 ExecuteIndex;
};

UCLASS(meta = (DisplayName = "行为容器_状态行为"))
class ARPG_API UCB_StateBehavior : public UCB_ContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_CharacterBehaviorBase* StartBehavior;

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_CharacterBehaviorBase* LoopBehavior;

	UPROPERTY(EditAnywhere, Category = "行为", Instanced)
	UARPG_CharacterBehaviorBase* EndBehavior;

	void WhenBehaviorExecuted(ACharacterBase* Executer) override;

	void WhenBehaviorAborted(ACharacterBase* Executer) override;
private:
	UPROPERTY()
	UARPG_CharacterBehaviorBase* CurrentBehavior;

	void WhenStartBehaviorEnd(bool Succeed, ACharacterBase* Executer);

	void WhenLoopBehaviorEnd(bool Succeed, ACharacterBase* Executer);

	void WhenEndBehaviorEnd(bool Succeed, ACharacterBase* Executer);

	void WhenAbortBehaviorEnd(ACharacterBase* Executer);
};
