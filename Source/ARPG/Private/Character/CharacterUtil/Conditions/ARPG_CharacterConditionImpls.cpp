// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterConditionImpls.h"
#include "CharacterBase.h"
#include "ARPG_MovementComponent.h"

bool UACC_OnLand::CalculateConditionValue_Implementation(class ACharacterBase* Character) const
{
	return Character->ARPG_MovementComponent->IsMovingOnGround();
}
