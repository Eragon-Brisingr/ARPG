// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Battle/ARPG_AI_BattleInterface.h"
#include "ARPG_AttackAreaManager.generated.h"

class ACharacterBase;
class UAnimMontage;

UCLASS(abstract, EditInlineNew, collapsecategories)
class ARPG_API UARPG_AttackParam : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	ACharacterBase* Attacker;

	UWorld* GetWorld() const override;

	virtual bool CanAttack() const {return true; }

	virtual void InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) {}

	virtual void AttackingTick(AActor* AttackTarget, float DeltaSecond) {}

	virtual void AbortAttack(AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) {}
};

USTRUCT()
struct ARPG_API FAttackAreaParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "配置")
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "配置", Instanced)
	UARPG_AttackParam* AttackParam;
};

UCLASS()
class ARPG_API UAP_NormalMontage : public UARPG_AttackParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "配置")
	UAnimMontage* Montage;

	void InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;

	void WhenMontageBlendOutStart(UAnimMontage* CurMontage, bool bInterrupted, FBP_OnAttackFinished OnAttackFinished);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_API UARPG_AttackAreaManager : public UActorComponent,
	public IARPG_AI_BattleInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARPG_AttackAreaManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FVector GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const override;

	bool CanAttack_Implementation(class AActor* AttackTarget) const override { return false; }

	void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;

	void AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) override;

	void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond) override;

	void WhenAttackFinished(bool Succeed, FBP_OnAttackFinished OnAttackFinished);

	UPROPERTY(EditAnywhere, Category = "配置")
	TArray<FAttackAreaParam> AttackArea;

	UPROPERTY()
	UARPG_AttackParam* ActiveAttackParam;
};
