// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AI_BattleInterface.h"
#include "GameFramework/Actor.h"

// Add default functionality here for any IARPG_AI_BattleInterface functions that are not pure virtual.

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
