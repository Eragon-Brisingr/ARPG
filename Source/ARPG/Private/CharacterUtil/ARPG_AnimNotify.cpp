// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AnimNotify.h"
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include <Animation/AimOffsetBlendSpace.h>
#include "CharacterBase.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "ReceiveDamageActionBase.h"
#include "XD_MacrosLibrary.h"
#include <GameFramework/CharacterMovementComponent.h>


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

void UARPG_SetMovementMode::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp))
	{
		switch (MovementMode)
		{
		case ESettableMovement::Fly:
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			break;
		}
	}
}

void UARPG_SetMovementMode::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp))
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

FString UARPG_SetMovementMode::GetNotifyName_Implementation() const
{
#if WITH_EDITOR
	if (MovementModeEnum == nullptr)
	{
		MovementModeEnum = FindObject<UEnum>(ANY_PACKAGE, *GET_TYPE_NAME_STRING_CHECKED(EMovementMode));
	}
	return FString::Printf(TEXT("移动模式[%s]"), *MovementModeEnum->GetNameStringByValue((int64)MovementMode));
#endif
	return FString();
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

void UARPG_ApplyExecuteDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ApplyExecuteDamageFunctor)
	{
		if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp))
		{
			if (Character->ExecuteTargetCharacter)
			{
				ApplyExecuteDamageFunctor.GetDefaultObject()->ApplyExecuteDamage(Character->ExecuteTargetCharacter, Character);
			}
		}
	}
}

FString UARPG_ApplyExecuteDamage::GetNotifyName_Implementation() const
{
	return TEXT("造成处决伤害");
}

void UARPG_SetReceiveDamageAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (SetReceiveDamageActionFunctor)
	{
		SetReceiveDamageActionFunctor.GetDefaultObject()->SetReceiveDamageAction(MeshComp, ReceiveDamageAction);
	}
}

FString UARPG_SetReceiveDamageAction::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("设置特殊受击动画"), *UARPG_DebugFunctionLibrary::GetDebugName(ReceiveDamageAction));
}

void UARPG_SetAddHitStunValue::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (SetAddHitStunValueFunctor)
	{
		SetAddHitStunValueFunctor.GetDefaultObject()->SetAddHitStunValue(MeshComp, AddHitStunValue);
	}
}

void UARPG_SetAddHitStunValue::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (SetAddHitStunValueFunctor)
	{
		SetAddHitStunValueFunctor.GetDefaultObject()->Reset(MeshComp, AddHitStunValue);
	}
}

FString UARPG_SetAddHitStunValue::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("设置动画增加削韧量[%s]"), AddHitStunValue);
}

void UARPG_SetBeakBackDistance::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (SetBeakBackDistanceFunctor)
	{
		SetBeakBackDistanceFunctor.GetDefaultObject()->SetBeakBackDistance(MeshComp, BeakBackDistance);
	}
}

FString UARPG_SetBeakBackDistance::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("设置击退距离[%s]"), BeakBackDistance);
}
