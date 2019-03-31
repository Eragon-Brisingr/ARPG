// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "ARPG_CharacterBehaviorType.h"
#include "ARPG_CharacterBehaviorBase.generated.h"

/**
 *  
 */
UCLASS(abstract, EditInlineNew, collapsecategories, Blueprintable)
class ARPG_API UARPG_CharacterBehaviorBase : public UObject
{
	GENERATED_BODY()
public:
	UWorld* GetWorld() const override;
public:
	UPROPERTY(BlueprintReadOnly, Category = "行为")
	class ACharacterBase* Character;

	UPROPERTY(BlueprintReadOnly, Category = "行为")
	uint8 bIsExecuting : 1;
public:
	void ExecuteBehavior(class ACharacterBase* Executer, const FOnCharacterBehaviorFinished& WhenBehaviorFinished);

	void AbortBehavior(class ACharacterBase* Executer, const FOnCharacterBehaviorAbortFinished& WhenBehaviorAbortFinished);
protected:
	virtual void WhenBehaviorExecuted(class ACharacterBase* Executer) { ReceiveWhenBehaviorExecuted(Executer); }
	UFUNCTION(BlueprintImplementableEvent, Category = "行为", meta = (DisplayName = "WhenBehaviorExecuted"))
	void ReceiveWhenBehaviorExecuted(class ACharacterBase* Executer);

protected:
	virtual void WhenBehaviorAborted(class ACharacterBase* Executer) { ReceiveWhenBehaviorAborted(Executer); }
	UFUNCTION(BlueprintNativeEvent, Category = "行为", meta = (DisplayName = "WhenBehaviorAborted"))
	void ReceiveWhenBehaviorAborted(class ACharacterBase* Executer);
	void ReceiveWhenBehaviorAborted_Implementation(class ACharacterBase* Executer) { OnBehaviorAbortFinished.ExecuteIfBound(); }
public:
	UFUNCTION(BlueprintCallable, Category = "行为")
	void FinishExecute(bool Succeed);

	FOnCharacterBehaviorFinished OnBehaviorFinished;

	UFUNCTION(BlueprintCallable, Category = "行为")
	void FinishAbort();

	FOnCharacterBehaviorAbortFinished OnBehaviorAbortFinished;

	virtual FString GetDescribe() const;
};

USTRUCT(BlueprintType)
struct ARPG_API FCharacterBehavior
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "行为", Instanced)
	class UARPG_CharacterBehaviorBase* Behavior = nullptr;
};
