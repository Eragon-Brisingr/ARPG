// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterAnimType.h"
#include "CharacterBase.h"
#include "ARPG_MovementComponent.h"
#include "ARPG_Anim_Log.h"
#include "ARPG_DebugFunctionLibrary.h"
#include <Package.h>

bool UARPG_AnimPlayCondition::CalculateConditionValue_Implementation(class ACharacterBase* Character) const
{
	return CanPlayMontage(Character);
}

void UARPG_AnimFunctionLibrary::CheckAndFixAnimData(UAnimMontage* Montage, TSubclassOf<class UAnimMetaData> AnimMetaType)
{
	if (Montage)
	{
		if (!Montage->GetMetaData().ContainsByPredicate([&](UAnimMetaData* E) {return E && E->IsA(AnimMetaType); }))
		{
			Montage->AddMetaData(NewObject<UAMD_CanPlayWhenFalling>(Montage));
			if (UPackage* Package = Montage->GetTypedOuter<UPackage>())
			{
				const bool bIsDirty = Package->IsDirty();
				if (!bIsDirty)
				{
					Package->SetDirtyFlag(true);
				}
				Package->PackageMarkedDirtyEvent.Broadcast(Package, bIsDirty);
			}
			Anim_Error_Log("%s中不存在%s元数据，自动添加", *UARPG_DebugFunctionLibrary::GetDebugName(Montage), *UARPG_DebugFunctionLibrary::GetDebugName(AnimMetaType));
		}
	}
}

void UARPG_AttackAnimSetNormal::InvokePlay_Implementation(class ACharacterBase* Character) const
{
	if (Character && Character->CanPlayFullBodyMontage())
	{
		switch (Character->ARPG_MovementComponent->MovementMode)
		{
		case EMovementMode::MOVE_Walking:
		case EMovementMode::MOVE_NavWalking:
		case EMovementMode::MOVE_Falling:
		{
			if (Character->ARPG_MovementComponent->IsCrouching())
			{

			}
			else
			{
				bool IsSprinting = Character->ARPG_MovementComponent->IsSprinting();
				bool IsFalling = Character->ARPG_MovementComponent->IsFalling();

				if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::LeftLightAttack))
				{
					if (IsSprinting)
					{
						Character->TryPlayMontage(LeftSprintAttack);
					}
					else if (IsFalling)
					{
						UARPG_AnimFunctionLibrary::CheckAndFixAnimData(LeftFallingAttack.Montage, UAMD_CanPlayWhenFalling::StaticClass());

						Character->TryPlayMontage(LeftFallingAttack);
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
					if (IsSprinting)
					{
   						Character->TryPlayMontage(RightSprintAttack);
					}
					else if (IsFalling)
					{
						UARPG_AnimFunctionLibrary::CheckAndFixAnimData(RightFallingAttack.Montage, UAMD_CanPlayWhenFalling::StaticClass());

						Character->TryPlayMontage(RightFallingAttack);
					}
					else
					{
						Character->TryPlayMontage(RightLightAttack);
					}
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

void UARPG_AttackAnimSetNormal::InvokePlayDodgeAnim_Implementation(class ACharacterBase* Character, EDodgeDirection DodgeDirection) const
{
	switch (Character->ARPG_MovementComponent->MovementMode)
	{
	case EMovementMode::MOVE_Walking:
	case EMovementMode::MOVE_NavWalking:
		if (Character->ARPG_AnyInputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::LeftLightAttack) | ARPG_InputType::ToBitMask(EARPG_InputType::LeftHeavyAttack)))
		{
			Character->TryPlayMontage(DodogeForwardLeftAttack);
		}
		else if (Character->ARPG_AnyInputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::RightLightAttack) | ARPG_InputType::ToBitMask(EARPG_InputType::RightHeavyAttack)))
		{
			Character->TryPlayMontage(DodogeForwardRightAttack);
		}
	}
}

void UARPG_DodgeAnimSetNormal::InvokeDodge(class ACharacterBase* Character, EDodgeDirection Direction) const
{
	if (CanDodge(Character))
	{
		switch (Direction)
		{
		case EDodgeDirection::Forward:
			Character->TryPlayMontage(DodgeForwardAnim);
			break;
		case EDodgeDirection::Backword:
			Character->TryPlayMontage(DodgeBackAnim);
			break;
		case EDodgeDirection::Left:
			Character->TryPlayMontage(DodgeLeftAnim);
			break;
		case EDodgeDirection::Right:
			Character->TryPlayMontage(DodgeRightAnim);
			break;
		}
	}
}

bool UARPG_DodgeAnimSetNormal::CanDodge(const class ACharacterBase* Character) const
{
	return Character->GetCharacterMovement()->IsMovingOnGround() && Character->CanPlayFullBodyMontage();
}
