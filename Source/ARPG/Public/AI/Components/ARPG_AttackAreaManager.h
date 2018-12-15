// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Battle/ARPG_AI_BattleInterface.h"
#include "ARPG_AttackAreaManager.generated.h"


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

	bool CanAttack_Implementation(class AActor* AttackTarget) const override { return false; }

	void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;

	void AbortAttack_Implementation(class AActor* AttackTarget) override;

	void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond) override;
};
