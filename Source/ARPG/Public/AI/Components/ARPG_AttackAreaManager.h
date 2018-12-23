// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Battle/ARPG_AI_BattleInterface.h"
#include "ARPG_AttackAreaManager.generated.h"

class ACharacterBase;
class UAnimMontage;

UCLASS(abstract, EditInlineNew, collapsecategories)
class ARPG_API UARPG_AttackExecuterBase : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY()
	ACharacterBase* Attacker;

	UWorld* GetWorld() const override;

	virtual bool CanAttack() const {return true; }

	virtual void InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) {}

	virtual void AttackingTick(AActor* AttackTarget, float DeltaSecond) {}

	virtual void AbortAttack(AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) { OnAttackAborted.ExecuteIfBound(); }

};

UCLASS(abstract, EditInlineNew, collapsecategories)
class ARPG_API UARPG_AttackAreaBase : public UObject
{
	GENERATED_BODY()
public:
	virtual FVector GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const { return FVector::ZeroVector; }

	virtual bool IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const { return false; }
};

UCLASS(abstract, EditInlineNew, collapsecategories)
class ARPG_API UARPG_AttackConditionBase : public UObject
{
	GENERATED_BODY()
public:
	virtual bool CanAttack(ACharacterBase* Attacker) const { return true; }

	virtual void WhenExecuteAttack(ACharacterBase* Attacker) {}
};

UCLASS(meta = (DisplayName = "冷却"))
class ARPG_API UAC_CoolDown : public UARPG_AttackConditionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "配置")
	float CoolDownTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "配置")
	float RandomRange = 2.f;

	float NextExecuteSecond = FLT_MIN;

	virtual bool CanAttack(ACharacterBase* Attacker) const;

	virtual void WhenExecuteAttack(ACharacterBase* Attacker);
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

UCLASS(meta = (DisplayName = "播放蒙太奇"))
class ARPG_API UAE_NormalMontage : public UARPG_AttackExecuterBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "配置")
		UAnimMontage* Montage;

	void InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;

	void WhenMontageBlendOutStart(UAnimMontage* CurMontage, bool bInterrupted, FBP_OnAttackFinished OnAttackFinished);
};

USTRUCT()
struct ARPG_API FAttackAreaParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "配置", Instanced)
	UARPG_AttackAreaBase* AttackArea;

	UPROPERTY(EditAnywhere, Category = "配置", Instanced)
	UARPG_AttackConditionBase* Condition;

	UPROPERTY(EditAnywhere, Category = "配置", Instanced)
	UARPG_AttackExecuterBase* AttackExecuter;

	bool CanAttack(ACharacterBase* Attacker, AActor* AttackTarget) const
	{
		bool Res = Condition ? Condition->CanAttack(Attacker) : true;
		if (Res)
		{
			Res = AttackArea ? AttackArea->IsInArea(Attacker, AttackTarget) : false;
			if (Res)
			{
				return AttackExecuter ? AttackExecuter->CanAttack() : false;
			}
		}
		return false;
	}
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

	bool IsAllowedAttack_Implementation(class AActor* AttackTarget) const override;

	void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;

	void AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) override;

	void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond) override;

	void WhenAttackFinished(bool Succeed, FBP_OnAttackFinished OnAttackFinished);

	UPROPERTY(EditAnywhere, Category = "配置")
	int32 MainAttackConfig;

	UPROPERTY(EditAnywhere, Category = "配置")
	TArray<FAttackAreaParam> AttackConfigs;

	mutable int32 ActiveAttackConfigIndex = INDEX_NONE;

	FORCEINLINE UARPG_AttackExecuterBase* GetActiveAttackParam() const { return ActiveAttackConfigIndex != INDEX_NONE ? AttackConfigs[ActiveAttackConfigIndex].AttackExecuter : nullptr; }

	FORCEINLINE UARPG_AttackAreaBase* GetActiveAttackArea() const { return ActiveAttackConfigIndex != INDEX_NONE ? AttackConfigs[ActiveAttackConfigIndex].AttackArea : nullptr; }

	UPROPERTY()
	ACharacterBase* Attacker;
};
