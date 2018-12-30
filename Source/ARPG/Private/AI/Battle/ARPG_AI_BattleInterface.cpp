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

void IARPG_AI_BattleInterface::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{
	OnAttackAborted.ExecuteIfBound();
}

void IARPG_AI_BattleInterface::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{

}

bool IARPG_AI_BattleInterface::CanDefense_Implementation(class AActor* DefenseTarget) const
{
	return false;
}

FVector IARPG_AI_BattleInterface::GetDefenseMoveLocation_Implementation(class AActor* DefenseTarget) const
{
	if (const AActor* Owner = Cast<AActor>(this))
	{
		return Owner->GetActorLocation();
	}
	return FVector::ZeroVector;
}

FRotator IARPG_AI_BattleInterface::GetDefenseFaceRotation_Implementation(class AActor* DefenseTarget) const
{
	if (const AActor* Owner = Cast<AActor>(this))
	{
		return (DefenseTarget->GetActorLocation() - Owner->GetActorLocation()).Rotation();
	}
	return FRotator::ZeroRotator;
}

bool IARPG_AI_BattleInterface::IsAllowedDefense_Implementation(class AActor* DefenseTarget) const
{
	return false;
}

void IARPG_AI_BattleInterface::InvokeDefense_Implementation(class AActor* DefenseTarget, const FBP_OnDefenseFinished& OnDefenseFinished)
{
	OnDefenseFinished.ExecuteIfBound(false);
}

void IARPG_AI_BattleInterface::AbortDefense_Implementation(class AActor* DefenseTarget, const FBP_OnDefenseAborted& OnDefenseAborted)
{
	OnDefenseAborted.ExecuteIfBound();
}

void IARPG_AI_BattleInterface::DefenseingTick_Implementation(class AActor* DefenseTarget, float DeltaSecond)
{

}

bool IARPG_AI_BattleInterface::CanDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget) const
{
	return false;
}

FVector IARPG_AI_BattleInterface::GetDefenseSwipeMoveLocation_Implementation(class AActor* DefenseSwipeTarget) const
{
	if (const AActor* Owner = Cast<AActor>(this))
	{
		return Owner->GetActorLocation();
	}
	return FVector::ZeroVector;
}

FRotator IARPG_AI_BattleInterface::GetDefenseSwipeFaceRotation_Implementation(class AActor* DefenseSwipeTarget) const
{
	if (const AActor* Owner = Cast<AActor>(this))
	{
		return (DefenseSwipeTarget->GetActorLocation() - Owner->GetActorLocation()).Rotation();
	}
	return FRotator::ZeroRotator;
}

bool IARPG_AI_BattleInterface::IsAllowedDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget) const
{
	return false;
}

void IARPG_AI_BattleInterface::InvokeDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeFinished& OnDefenseSwipeFinished)
{
	OnDefenseSwipeFinished.ExecuteIfBound(false);
}

void IARPG_AI_BattleInterface::AbortDefenseSwipe_Implementation(class AActor* DefenseSwipeTarget, const FBP_OnDefenseSwipeAborted& OnDefenseSwipeAborted)
{
	OnDefenseSwipeAborted.ExecuteIfBound();
}

void IARPG_AI_BattleInterface::DefenseSwipeingTick_Implementation(class AActor* DefenseSwipeTarget, float DeltaSecond)
{

}
