// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_MontagePlayCondition_Impl.h"
#include "CharacterBase.h"
#include "ARPG_MovementComponent.h"



bool UAPC_FallingAttack_OnLand::CanPlayMontage_Implementation(class ACharacterBase* Character) const
{
	return Character->ARPG_MovementComponent->IsMovingOnGround();
}

bool UAPC_Character_Alive::CanPlayMontage_Implementation(class ACharacterBase* Character) const
{
	return Character->IsAlive();
}
