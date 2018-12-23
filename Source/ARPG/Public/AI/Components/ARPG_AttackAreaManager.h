// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Battle/ARPG_AI_BattleInterface.h"
#include "ARPG_AttackAreaManager.generated.h"

class ACharacterBase;
class UAnimMontage;

UCLASS(abstract, EditInlineNew, collapsecategories)
class ARPG_API UARPG_AttackParamBase : public UObject
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

UCLASS(abstract, EditInlineNew, collapsecategories)
class ARPG_API UARPG_AttackAreaBase : public UObject
{
	GENERATED_BODY()
public:
	virtual FVector GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const { return FVector::ZeroVector; }

	virtual bool IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const { return false; }
};

UCLASS(meta = (DisplayName = "球体"))
class ARPG_API UARPG_AttackArea_Sphere : public UARPG_AttackAreaBase
{
	GENERATED_BODY()
public:
	FVector GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const override;

	bool IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const override;
	
	UPROPERTY(EditAnywhere, Category = "配置")
	FVector Origin;

	UPROPERTY(EditAnywhere, Category = "配置")
	float Radius = 100.f;

	float GetWorldRadius(const FVector& OwnerWorldScale) const;
};

UCLASS(meta = (DisplayName = "盒体"))
class ARPG_API UARPG_AttackArea_Box : public UARPG_AttackAreaBase
{
	GENERATED_BODY()
public:
	FVector GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const override;

	bool IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const override;
	
	UPROPERTY(EditAnywhere, Category = "配置")
	FVector Origin;
	
	UPROPERTY(EditAnywhere, Category = "配置")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, Category = "配置")
	FVector Extent = FVector(100.f);
};


USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = true))
struct ARPG_API FAttackAreaParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "配置", Instanced)
	UARPG_AttackAreaBase* AttackArea;

	UPROPERTY(EditAnywhere, Category = "配置", Instanced)
	UARPG_AttackParamBase* AttackParam;
};

UCLASS()
class ARPG_API UAP_NormalMontage : public UARPG_AttackParamBase
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
	int32 MainAttackConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "配置")
	TArray<FAttackAreaParam> AttackConfigs;

	int32 ActiveAttackConfigIndex;

	FORCEINLINE UARPG_AttackParamBase* GetActiveAttackParam() const { return ActiveAttackConfigIndex != INDEX_NONE ? AttackConfigs[ActiveAttackConfigIndex].AttackParam : nullptr; }

	FORCEINLINE UARPG_AttackAreaBase* GetActiveAttackArea() const { return ActiveAttackConfigIndex != INDEX_NONE ? AttackConfigs[ActiveAttackConfigIndex].AttackArea : nullptr; }

	UPROPERTY()
	ACharacterBase* Attacker;
};
