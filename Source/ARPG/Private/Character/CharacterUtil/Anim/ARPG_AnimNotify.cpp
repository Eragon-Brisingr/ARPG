﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AnimNotify.h"
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include <Animation/AimOffsetBlendSpace.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "CharacterBase.h"
#include "ARPG_DebugFunctionLibrary.h"
#include "ReceiveDamageActionBase.h"
#include "XD_MacrosLibrary.h"
#include "ARPG_ItemBase.h"
#include "ARPG_ItemCoreBase.h"


void UARPG_PlayMontageByState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase * Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		FARPG_MontagePlayConfig Config;						
		if (bInheritMirrorMontage)
		{
			Config.bMirrorMontage = Character->bMirrorFullBodyMontage;
		}
		Character->TryPlayMontage(MontageParameter, Config);
	}
}

FString UARPG_PlayMontageByState::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("动画跳转[%s]"), MontageParameter.Montage ? *MontageParameter.Montage->GetName() : TEXT("None"));
}

void UARPG_PlayMontageCheckState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (UAnimMontage* CurPlayingMontage = Cast<UAnimMontage>(Animation))
		{
			if (MeshComp->GetAnimInstance()->Montage_IsPlaying(CurPlayingMontage))
			{
				FARPG_MontagePlayConfig Config;
				if (bInheritMirrorMontage)
				{
					Config.bMirrorMontage = Character->bMirrorFullBodyMontage;
				}
				Character->TryPlayMontage(MontageParameter, Config);
			}
		}
	}
}

FString UARPG_PlayMontageCheckState::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("动画跳转[%s]"), MontageParameter.Montage ? *MontageParameter.Montage->GetName() : TEXT("None"));
}

void UARPG_PlayMontageByInput::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}

void UARPG_PlayMontageByInput::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (Character->IsLocallyControlled())
		{
			int32 CheckedInputType = bInheritMirrorMontage && Character->bMirrorFullBodyMontage ? ARPG_InputType::MirrorAttackBitMask(InputType) : InputType;
			if (bIsReleased ? Character->ARPG_InputIsReleased(CheckedInputType) : Character->ARPG_InputIsPressed(CheckedInputType))
			{
				if (UAnimMontage* CurPlayingMontage = Cast<UAnimMontage>(Animation))
				{
					if (MeshComp->GetAnimInstance()->Montage_IsPlaying(CurPlayingMontage))
					{
						FARPG_MontagePlayConfig Config;
						if (bInheritMirrorMontage)
						{
							Config.bMirrorMontage = Character->bMirrorFullBodyMontage;
						}
						Character->TryPlayMontage(MontageParameter, Config);
					}
				}
			}
		}
	}
}

void UARPG_PlayMontageByInput::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

}

FString UARPG_PlayMontageByInput::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("输入_动画跳转[%s]"), MontageParameter.Montage ? *MontageParameter.Montage->GetName() : TEXT("None"));
}

void UARPG_DodgeAnim::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->InvokeDodge();
	}
}

FString UARPG_DodgeAnim::GetNotifyName_Implementation() const
{
	return TEXT("输入_闪避");
}

void UARPG_DefenseState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (DefenseStateFunctor)
	{
		DefenseStateFunctor.GetDefaultObject()->DefenseBegin(MeshComp);
	}
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefense = true;
	}
}

void UARPG_DefenseState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (DefenseStateFunctor)
	{
		DefenseStateFunctor.GetDefaultObject()->DefenseEnd(MeshComp);
	}
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefense = false;
	}
}

FString UARPG_DefenseState::GetNotifyName_Implementation() const
{
	return TEXT("防御状态");
}

void UARPG_DefenseSwipeState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDefenseSwipe = true;
	}
}

void UARPG_DefenseSwipeState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

void UARPG_DodgeState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bIsDodging = true;
	}
}

void UARPG_DodgeState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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

UARPG_EnableAttackTracer::UARPG_EnableAttackTracer()
{
#if WITH_EDITOR
	PreviewSocketMoveTracer = NewObject<USocketMoveTracer>();
#endif
}

void UARPG_EnableAttackTracer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		USocketMoveTracer*& ValidSocketMoveTracer = Character->SocketMoveTracerMap.FindOrAdd(this);
		if (ValidSocketMoveTracer == nullptr)
		{
			for (TPair<UAnimNotifyState*, USocketMoveTracer*>& Pair : Character->SocketMoveTracerMap)
			{
				if (USocketMoveTracer* SocketMoveTracer = Pair.Value)
				{
					if (SocketMoveTracer->bEnableTrace == false)
					{
						ValidSocketMoveTracer = SocketMoveTracer;
						Pair.Value = nullptr;
						break;
					}
				}
			}

			if (ValidSocketMoveTracer == nullptr)
			{
				ValidSocketMoveTracer = NewObject<USocketMoveTracer>(Character);
			}
		}

		ValidSocketMoveTracer->InitSocketMoveTracer(MeshComp, SocketMoveTracerConfig);
		ValidSocketMoveTracer->OnTraceActorNative.BindLambda([=](UPrimitiveComponent* HitComponent, const FName& SocketName, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& TraceResult)
		{
			if (ACharacterBase* DamagedCharacter = Cast<ACharacterBase>(OtherActor))
			{
				float CauseDamage = DamagedCharacter->ApplyPointDamage(100.f, (DamagedCharacter->GetActorLocation() - TraceResult.ImpactPoint).GetSafeNormal(), TraceResult, FARPG_PropertyChangeContext(Character, Character), nullptr, PointDamageParameter);
				if (CauseDamage > 0.f)
				{
					DamagedCharacter->NearAttackSuccessTimeDilation(0.2f);
				}
			}
		});
		ValidSocketMoveTracer->EnableTrace(true);
	}

#if WITH_EDITOR
	if (MeshComp->GetWorld()->IsGameWorld() == false)
	{
		PreviewSocketMoveTracer->InitSocketMoveTracer(MeshComp, SocketMoveTracerConfig);
	}
#endif
}

#if WITH_EDITOR
void UARPG_EnableAttackTracer::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp->GetWorld()->IsGameWorld() == false)
	{
		PreviewSocketMoveTracer->DoTrace(FrameDeltaTime);
	}
}
#endif

void UARPG_EnableAttackTracer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (USocketMoveTracer* ValidSocketMoveTracer = Character->SocketMoveTracerMap.FindOrAdd(this))
		{
			ValidSocketMoveTracer->DisableTrace();
		}
	}
}

FString UARPG_EnableAttackTracer::GetNotifyName_Implementation() const
{
	return TEXT("启用攻击检测");
}

void UARPG_SetMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
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

void UARPG_SetMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
		MovementModeEnum = FindObject<UEnum>(ANY_PACKAGE, *GET_TYPE_CPP_NAME_STRING_CHECKED(ESettableMovement));
	}
	return FString::Printf(TEXT("移动模式[%s]"), *MovementModeEnum->GetDisplayNameTextByValue((int64)MovementMode).ToString());
#endif
	return FString();
}

void UARPG_AddToughnessValue::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->Toughness.PushAdditiveModifier(FARPG_FloatProperty_ModifyConfig(AddToughnessValue, GetFName(), this, Character));
	}
}

void UARPG_AddToughnessValue::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->Toughness.PopAdditiveModifier(FARPG_FloatProperty_ModifyConfig(AddToughnessValue, GetFName(), this, Character));
	}
}

FString UARPG_AddToughnessValue::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("增加强韧度[%s]"), *FString::SanitizeFloat(AddToughnessValue, 0));
}

void UARPG_SetAimOffsetOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (AimOffsetOverride)
		{
			Character->AimOffsetOverride = AimOffsetOverride;
		}
		else
		{
			Character->bEnableAimOffset = false;
		}
	}
}

void UARPG_SetAimOffsetOverride::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		Character->bEnableAimOffset = true;
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

void UARPG_SetAddHitStunValue::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (SetAddHitStunValueFunctor)
	{
		SetAddHitStunValueFunctor.GetDefaultObject()->SetAddHitStunValue(MeshComp, AddHitStunValue);
	}
}

void UARPG_SetAddHitStunValue::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
		SetBeakBackDistanceFunctor.GetDefaultObject()->SetBeakBackDistance(MeshComp, NormalBeakBackDistance, DefenseBeakBackDistance);
	}
}

FString UARPG_SetBeakBackDistance::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("设置击退距离[%s]"), NormalBeakBackDistance);
}

UARPG_SetAttackInfo::UARPG_SetAttackInfo()
{
	AttackInfos.Add(FARPG_AttackInfo());
}

void UARPG_SetAttackInfo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		for (const FARPG_AttackInfo& AttackInfo : AttackInfos)
		{
			Character->AddAttackInfo(AttackInfo);
		}
	}
}

void UARPG_SetAttackInfo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		for (const FARPG_AttackInfo& AttackInfo : AttackInfos)
		{
			Character->RemoveAttackInfo(AttackInfo);
		}
	}
}

FString UARPG_SetAttackInfo::GetNotifyName_Implementation() const
{
	return TEXT("设置攻击信息");
}

#if WITH_EDITORONLY_DATA
bool CanShowDebugHint(EARPG_AnimDebugHintType DebugHintType, USkeletalMeshComponent* MeshComp)
{
	return (DebugHintType == EARPG_AnimDebugHintType::OnlyPreview && MeshComp->GetWorld()->IsPreviewWorld()) || DebugHintType == EARPG_AnimDebugHintType::All;
}

void UARPG_SetAttackInfo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (CanShowDebugHint(DebugHintType, MeshComp))
	{
		for (const FARPG_AttackInfo& AttackInfo : AttackInfos)
		{
			UKismetSystemLibrary::DrawDebugSphere(MeshComp, MeshComp->GetSocketTransform(AttackInfo.SocketName).TransformPosition(AttackInfo.Offset), AttackInfo.Radius, 12, NotifyColor, FrameDeltaTime * 1.5f, 1.f);
		}
	}
}
#endif

void UARPG_InteractEventNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{

	}
}

FString UARPG_InteractEventNotify::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("交互_事件[%s]"), *EventTag.ToString());
}

void UARPG_InteractEventNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{

	}
}

void UARPG_InteractEventNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{

	}
}

FString UARPG_InteractEventNotifyState::GetNotifyName_Implementation() const
{
	return FString::Printf(TEXT("交互_事件[%s]"), *EventTag.ToString());
}

void UARPG_UsePendingItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (Character->PendingUseItem)
		{
			Character->UseItemImmediately(CastChecked<UARPG_ItemCoreBase>(Character->PendingUseItem->GetItemCoreConst()));
			if (bDestroyItem)
			{
				Character->PendingUseItem->Destroy();
				Character->PendingUseItem = nullptr;
			}
		}
	}
}

FString UARPG_UsePendingItem::GetNotifyName_Implementation() const
{
	return TEXT("事件_使用道具");
}

void UARPG_ShowUsePendingItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ACharacterBase* Character = Cast<ACharacterBase>(MeshComp->GetOwner()))
	{
		if (Character->PendingUseItem)
		{
			Character->PendingUseItem->SetActorHiddenInGame(false);
		}
	}
}

FString UARPG_ShowUsePendingItem::GetNotifyName_Implementation() const
{
	return TEXT("事件_显示正在使用的道具");
}
