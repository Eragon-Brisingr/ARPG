﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterAnimType.h"
#include "CharacterBase.h"
#include "ARPG_MovementComponent.h"




void UARPG_AttackAnimSetNormal::InvokePlay_Implementation(class ACharacterBase* Character) const
{
	if (Character && Character->CanPlayFullBodyMontage())
	{
		switch (Character->ARPG_MovementComponent->MovementMode)
		{
		case EMovementMode::MOVE_Walking:
		case EMovementMode::MOVE_NavWalking:
		{
			bool IsSprinting = Character->ARPG_MovementComponent->IsSprinting();

			if (Character->ARPG_MovementComponent->IsCrouching())
			{

			}
			else
			{
				if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::LeftLightAttack))
				{
					if (IsSprinting)
					{
						Character->TryPlayMontage(SprintAttack);
					}
					else
					{
						Character->TryPlayMontage(LeftLightAttack);
					}
				}
				else if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::LeftHeavyAttack))
				{
					Character->TryPlayMontage(LeftHeavyAttack);
				}
				else if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::RightLightAttack))
				{
					Character->TryPlayMontage(RightLightAttack);
				}
				else if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::RightHeavyAttack))
				{
					Character->TryPlayMontage(RightHeavyAttack);
				}
			}
		}
		break;
		}
	}
}
