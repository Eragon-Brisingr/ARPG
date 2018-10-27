// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AnimNotify.h"
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include <Animation/AimOffsetBlendSpace.h>
#include "CharacterBase.h"
#include "HumanBase.h"
#include "ARPG_WeaponBase.h"
#include "ARPG_BowBase.h"
#include "ARPG_ArrowBase.h"



void UARPG_PlayMontageByState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (bClientMaster || Character->HasAuthority())
		{
			Character->PlayMontage(Montage, 1.f, StartSectionName, bClientMaster);
		}
	}
}

FString UARPG_PlayMontageByState::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("动画跳转[%s]"), Montage ? *Montage->GetName() : TEXT("None"));
}

void UARPG_PlayMontageCheckState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (UAnimMontage* CurPlayingMontage = Cast<UAnimMontage>(Animation))
		{
			if (MeshComp->GetAnimInstance()->Montage_IsPlaying(CurPlayingMontage))
			{
				if (Condition == nullptr || Condition.GetDefaultObject()->CanPlayMontage(Character))
				{
					Character->PlayMontage(Montage, 1.f, StartSectionName, bClientMaster);
				}
			}
		}
	}
}

FString UARPG_PlayMontageCheckState::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("动画跳转[%s]"), Montage ? *Montage->GetName() : TEXT("None"));
}

void UARPG_PlayMontageByInput::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{

}

void UARPG_PlayMontageByInput::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (Character->IsLocallyControlled())
		{
			if (bIsReleased ? Character->ARPG_InputIsReleased(InputType) : Character->ARPG_InputIsPressed(InputType))
			{
				if (UAnimMontage* CurPlayingMontage = Cast<UAnimMontage>(Animation))
				{
					if (MeshComp->GetAnimInstance()->Montage_IsPlaying(CurPlayingMontage))
					{
						Character->PlayMontage(Montage, 1.f, StartSectionName, bClientMaster);
					}
				}
			}
		}
	}
}

void UARPG_PlayMontageByInput::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{

}

FString UARPG_PlayMontageByInput::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("输入_动画跳转[%s]"), Montage ? *Montage->GetName() : TEXT("None"));
}

void UARPG_DefenseState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefense = true;
	}
}

void UARPG_DefenseState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefense = false;
	}
}

FString UARPG_DefenseState::GetNotifyName_Implementation() const
{
	return TEXT("防御状态");
}

void UARPG_DefenseSwipeState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefenseSwipe = true;
	}
}

void UARPG_DefenseSwipeState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefenseSwipe = false;
	}
}

FString UARPG_DefenseSwipeState::GetNotifyName_Implementation() const
{
	return TEXT("防御反击状态");
}

void UARPG_DodgeState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDodging = true;
	}
}

void UARPG_DodgeState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDodging = false;
	}
}

FString UARPG_DodgeState::GetNotifyName_Implementation() const
{
	return TEXT("闪避状态");
}

void UARPG_DodgeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (Human->IsLocallyControlled())
		{
			if (UAnimMontage* CurPlayingMontage = Cast<UAnimMontage>(Animation))
			{
				if (MeshComp->GetAnimInstance()->Montage_IsPlaying(CurPlayingMontage))
				{
					if (Human->ARPG_AnyInputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::LeftLightAttack) | ARPG_InputType::ToBitMask(EARPG_InputType::LeftHeavyAttack)))
					{
						if (Human->LeftWeapon)
						{
							Human->LeftWeapon->AttackAnimSet->InvokePlayDodgeAnim(Human, DodgeDirection);
						}
					}
					else if (Human->ARPG_AnyInputIsPressed(ARPG_InputType::ToBitMask(EARPG_InputType::RightLightAttack) | ARPG_InputType::ToBitMask(EARPG_InputType::RightHeavyAttack)))
					{
						if (Human->RightWeapon)
						{
							Human->RightWeapon->AttackAnimSet->InvokePlayDodgeAnim(Human, DodgeDirection);
						}
					}
				}
			}
		}
	}
}

FString UARPG_DodgeAttack::GetNotifyName_Implementation() const
{
	return TEXT("闪避攻击");
}

void UARPG_AddToughnessValue::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->ToughnessValue += AddToughnessValue;
	}
}

void UARPG_AddToughnessValue::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->ToughnessValue -= AddToughnessValue;
	}
}

FString UARPG_AddToughnessValue::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("增加强韧度[%s]"), *FString::SanitizeFloat(AddToughnessValue, 0));
}

void UARPG_SetAimOffsetOverride::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->AimOffsetOverride = AimOffsetOverride;
	}
}

void UARPG_SetAimOffsetOverride::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->AimOffsetOverride = nullptr;
	}
}

FString UARPG_SetAimOffsetOverride::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("瞄准偏移为[%s]"), AimOffsetOverride ? *AimOffsetOverride->GetName() : TEXT("None"));
}

void UARPG_Human_TakeWeaponPos::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (bPullOutWeapon)
		{
			Human->LetTheWeaponInHand();
		}
		else
		{
			Human->LetTheWeaponInWeaponBack();
		}
	}
}

FString UARPG_Human_TakeWeaponPos::GetNotifyName_Implementation() const
{
	return TEXT("武器位置");
}

void UARPG_Human_WeaponTrace::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			Weapon->ReceiveDamageAction = ReceiveDamageAction;
			Weapon->SetEnableNearAttackTrace(true);
		}
	}
}

void UARPG_Human_WeaponTrace::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			Weapon->ReceiveDamageAction = nullptr;
			Weapon->SetEnableNearAttackTrace(false);
		}
	}
}

FString UARPG_Human_WeaponTrace::GetNotifyName_Implementation() const
{
	return TEXT("武器伤害检测");
}

void UARPG_Human_FallingAttackTrace::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			Weapon->SetEnableFallingAttackTrace(true, bClearIgnoreList);
		}
	}
}

void UARPG_Human_FallingAttackTrace::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			Weapon->SetEnableFallingAttackTrace(false, bClearIgnoreList);
		}
	}
}

void UARPG_Human_PullOutArrow::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			if (AARPG_BowBase* Bow = Cast<AARPG_BowBase>(Weapon))
			{
				Bow->SpawnArrowInHand();
				Bow->HoldingTime = 0.f;
			}
		}
	}
}

void UARPG_Human_PullOutArrow::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			if (AARPG_BowBase* Bow = Cast<AARPG_BowBase>(Weapon))
			{
				if (Bow->HoldingArrow)
				{
					Bow->HoldingArrow->Destroy();
					Bow->HoldingArrow = nullptr;
				}
			}
		}
	}
}

void UARPG_Human_PullBow::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			if (AARPG_BowBase* Bow = Cast<AARPG_BowBase>(Weapon))
			{
				Bow->HoldingTime += FrameDeltaTime;
			}
		}
	}
}

void UARPG_Human_LaunchArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AHumanBase* Human = Cast<AHumanBase>(MeshComp->GetOwner()))
	{
		if (AARPG_WeaponBase* Weapon = bIsLeftWeapon ? Human->LeftWeapon : Human->RightWeapon)
		{
			if (AARPG_BowBase* Bow = Cast<AARPG_BowBase>(Weapon))
			{
				Bow->LaunchArrow(FullBowTime);
			}
		}
	}
}
