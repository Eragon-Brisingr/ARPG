// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AttackAreaManager.h"
#include "CharacterBase.h"

UWorld* UARPG_AttackParam::GetWorld() const
{
	return Attacker ? Attacker->GetWorld() : nullptr;
}

void UAP_NormalMontage::InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	Attacker->PlayMontageWithBlendingOutDelegate(Montage, FOnMontageBlendingOutStarted::CreateUObject(this, &UAP_NormalMontage::WhenMontageBlendOutStart, OnAttackFinished));
}

void UAP_NormalMontage::WhenMontageBlendOutStart(UAnimMontage* CurMontage, bool bInterrupted, FBP_OnAttackFinished OnAttackFinished)
{
	OnAttackFinished.ExecuteIfBound(!bInterrupted);
}

// Sets default values for this component's properties
UARPG_AttackAreaManager::UARPG_AttackAreaManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bAutoActivate = true;
}


// Called when the game starts
void UARPG_AttackAreaManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
	{
		if (bAutoActivate)
		{
			Character->BattleControl = this;
		}

		for (FAttackAreaParam& AttackAreaParam : AttackArea)
		{
			if (AttackAreaParam.AttackParam)
			{
				AttackAreaParam.AttackParam->Attacker = Character;
			}
		}
	}

}


// Called every frame
void UARPG_AttackAreaManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UARPG_AttackAreaManager::GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const
{
	FVector TargetLocation = AttackTarget->GetActorLocation();
	return TargetLocation + (GetOwner()->GetActorLocation() - TargetLocation).GetSafeNormal2D() * 300.f;
}

void UARPG_AttackAreaManager::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	for (const FAttackAreaParam& AttackAreaParam : AttackArea)
	{
		if (AttackAreaParam.AttackParam)
		{
			ActiveAttackParam = AttackAreaParam.AttackParam;
			AttackAreaParam.AttackParam->InvokeAttack(AttackTarget, FARPG_OnAttackFinished::CreateUObject(this, &UARPG_AttackAreaManager::WhenAttackFinished, OnAttackFinished));
		}
	}
}

void UARPG_AttackAreaManager::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{

}

void UARPG_AttackAreaManager::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{
	if (ActiveAttackParam)
	{
		ActiveAttackParam->AttackingTick(AttackTarget, DeltaSecond);
	}
}

void UARPG_AttackAreaManager::WhenAttackFinished(bool Succeed, FBP_OnAttackFinished OnAttackFinished)
{
	ActiveAttackParam = nullptr;
	OnAttackFinished.ExecuteIfBound(Succeed);
}
