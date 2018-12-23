// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AI_BattleInterface.h"
#include "GameFramework/Actor.h"

// Add default functionality here for any IARPG_AI_BattleInterface functions that are not pure virtual.

bool IARPG_AI_BattleInterface::CanAttack_Implementation(class AActor* AttackTarget) const
{
	return false;
}

FVector IARPG_AI_BattleInterface::GetAttackMoveLocation_Implementation(class AActor* Enemy) const
{
	return Enemy->GetActorLocation();
}

FRotator IARPG_AI_BattleInterface::GetAttackFaceRotation_Implementation(class AActor* Enemy) const
{
	if (const AActor* Actor = Cast<const AActor>(this))
	{
		return (Enemy->GetActorLocation() - Actor->GetActorLocation()).Rotation();
	}
	return FRotator::ZeroRotator;
}

bool IARPG_AI_BattleInterface::IsAllowedAttack_Implementation(class AActor* AttackTarget) const
{
	return false;
}

void IARPG_AI_BattleInterface::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	OnAttackFinished.ExecuteIfBound(false);
}

void IARPG_AI_BattleInterface::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{

}

void IARPG_AI_BattleInterface::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{
	OnAttackAborted.ExecuteIfBound();
}
