// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_CharacterStateBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
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

UCLASS(Blueprintable)
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

UENUM()
enum class EBuffOverlapType
{
	CanNotOverlap,
	Overlap,
	BothEffect,
	MixEffect
};

UCLASS(Blueprintable)
class ARPG_API UARPG_CharacterState_BuffBase : public UARPG_CharacterStateBase
{
	GENERATED_BODY()
public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuffOverlapType BuffOverlapType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IntervalTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DurationTime = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Replicated)
	float ContinuedTime = 0.f;

	void Active() { WhenActived(); }
	void Tick(float DeltaTime) { WhenTick(DeltaTime); }
	void Deactive() { WhenDeactived(); }
protected:
	virtual void WhenActived() { ReceiveWhenActived(); }
	virtual void WhenTick(float DeltaTime) { ReceiveWhenTick(DeltaTime); }
	virtual void WhenDeactived() { ReceiveWhenDeactived(); }

	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenActived"))
	void ReceiveWhenActived();
	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenTick"))
	void ReceiveWhenTick(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "Buff", meta = (DisplayName = "WhenDeactived"))
	void ReceiveWhenDeactived();
};
