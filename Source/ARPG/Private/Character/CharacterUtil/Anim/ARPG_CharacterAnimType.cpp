// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CharacterAnimType.h"
#include <Package.h>
#include "ARPG_MovementComponent.h"
#include "CharacterBase.h"
#include "ARPG_Anim_Log.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "HumanBase.h"
#include "HumanType.h"

bool UARPG_AnimPlayCondition::CalculateConditionValue_Implementation(class ACharacterBase* Character) const
{
	return CanPlayMontage(Character);
}

void FARPG_MontageParameterMirrorable::PlayLeftMontage(ACharacterBase* Character) const
{
	FARPG_MontagePlayConfig Config;
	Config.bMirrorMontage = !bNotMirrorFromRight;
	Character->TryPlayMontage(bNotMirrorFromRight ? LeftMontage : RightMontage, Config);
}

void FARPG_MontageParameterMirrorable::PlayRightMontage(ACharacterBase* Character) const
{
	FARPG_MontagePlayConfig Config;
	Config.bMirrorMontage = !bNotMirrorFromLeft;
	Character->TryPlayMontage(bNotMirrorFromLeft ? RightMontage : LeftMontage, Config);
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
		if (AHumanBase* Human = Cast<AHumanBase>(Character))
		{
			if (Human->UseWeaponState == EUseWeaponState::NoneWeapon_Default)
			{
				return;
			}
		}

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
						SprintAttack.PlayLeftMontage(Character);
					}
					else if (IsFalling)
					{
						UARPG_AnimFunctionLibrary::CheckAndFixAnimData(LeftFallingAttack.Montage, UAMD_CanPlayWhenFalling::StaticClass());

						FallingAttack.PlayLeftMontage(Character);
					}
					else
					{
						LightAttack.PlayLeftMontage(Character);
					}
				}
				else if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::LeftHeavyAttack))
				{
					HeavyAttack.PlayLeftMontage(Character);
				}
				else if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::RightLightAttack))
				{
					if (IsSprinting)
					{
						SprintAttack.PlayRightMontage(Character);
					}
					else if (IsFalling)
					{
						UARPG_AnimFunctionLibrary::CheckAndFixAnimData(RightFallingAttack.Montage, UAMD_CanPlayWhenFalling::StaticClass());

						FallingAttack.PlayRightMontage(Character);
					}
					else
					{
						LightAttack.PlayRightMontage(Character);
					}
				}
				else if (Character->ARPG_InputIsPressed(1 << (uint8)EARPG_InputType::RightHeavyAttack))
				{
					HeavyAttack.PlayRightMontage(Character);
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
			DodogeForwardAttack.PlayLeftMontage(Character);
		}
		else if (Character->ARPG_AnyInputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::RightLightAttack) | ARPG_InputType::ToBitMask(EARPG_InputType::RightHeavyAttack)))
		{
			DodogeForwardAttack.PlayRightMontage(Character);
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
			Character->TryPlayMontage(DodgeForwardAnim, {});
			break;
		case EDodgeDirection::Backword:
			Character->TryPlayMontage(DodgeBackAnim, {});
			break;
		case EDodgeDirection::Left:
			Character->TryPlayMontage(DodgeLeftAnim, {});
			break;
		case EDodgeDirection::Right:
			Character->TryPlayMontage(DodgeRightAnim, {});
			break;
		}
	}
}

bool UARPG_DodgeAnimSetNormal::CanDodge(const class ACharacterBase* Character) const
{
	return Character->GetCharacterMovement()->IsMovingOnGround() && Character->CanPlayFullBodyMontage();
}
