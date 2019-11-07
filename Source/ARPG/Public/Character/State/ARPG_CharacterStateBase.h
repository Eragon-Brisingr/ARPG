// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_CharacterStateBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "状态"))
class ARPG_API UARPG_CharacterStateBase : public UObject
{
	GENERATED_BODY()
public:
	UWorld* GetWorld() const override;
	bool IsSupportedForNetworking() const override { return true; }
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(BlueprintReadOnly)
	ACharacterBase* Owner;
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<ACharacterBase> Instigator;
};

UCLASS(meta = (DisplayName = "积累状态"))
class ARPG_API UARPG_CharacterState_AccumulationBase : public UARPG_CharacterStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	float AccumulationValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttenuationSpeed = 10.f;

	virtual float GetOverflowValue() const { return 100.f; }
	void Overflow() { WhenOverflow(); }
protected:
	virtual void WhenOverflow() { ReveiveWhenOverflow(); }

	UFUNCTION(BlueprintImplementableEvent, Category = "Accumulation", meta = (DisplayName = "WhenOverflow"))
	void ReveiveWhenOverflow();
};

UCLASS(meta = (DisplayName = "Buff"))
class ARPG_API UARPG_CharacterState_BuffBase : public UARPG_CharacterStateBase
{
	GENERATED_BODY()
public:
	UARPG_CharacterState_BuffBase();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff", SaveGame, Replicated)
	float IntervalTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff", SaveGame, Replicated)
	float DurationTime = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Buff", SaveGame, Replicated)
	float ContinuedTime = 0.f;

	// 为true则允许相同类型的BUFF叠加
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff")
	uint8 bAllowMulitSameTypeBuff : 1;
	void RepeatActive() { WhenRepeatActive(); }

	void Active(bool IsFirstInit) { WhenActived(IsFirstInit); }
	void Tick(float DeltaTime) { WhenTick(DeltaTime); }
	void Deactive() { WhenDeactived(); }
protected:
	virtual void WhenActived(bool IsFirstInit) { ReceiveWhenActived(IsFirstInit); }
	virtual void WhenTick(float DeltaTime) { ReceiveWhenTick(DeltaTime); }
	virtual void WhenDeactived() { ReceiveWhenDeactived(); }
	virtual void WhenRepeatActive() { ReceiveWhenRepeatActive(); }

	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenActived"))
	void ReceiveWhenActived(bool IsFirstInit);
	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenTick"))
	void ReceiveWhenTick(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenDeactived"))
	void ReceiveWhenDeactived();
	// 假如AllowMulitSameTypeBuff为false时添加相同类型的BUFF会触发该函数
	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenRepeatActive"))
	void ReceiveWhenRepeatActive();
};
