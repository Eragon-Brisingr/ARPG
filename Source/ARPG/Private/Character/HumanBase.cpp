// Fill out your copyright notice in the Description page of Project Settings.

#include "HumanBase.h"
#include <UnrealNetwork.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Animation/AnimInstance.h>
#include <TimerManager.h>
#include "ARPG_MovementComponent.h"
#include "ARPG_WeaponBase.h"
#include "ARPG_ItemCoreBase.h"
#include "ARPG_InventoryComponent.h"
#include "ARPG_EquipmentBase.h"
#include "ARPG_WeaponCoreBase.h"
#include "ARPG_EquipmentCoreBase.h"
#include "ARPG_ArrowBase.h"
#include "ARPG_ArrowCoreBase.h"
#include "Human_EnterReleaseState.h"
#if WITH_EDITOR
#include "MessageLog.h"
#include "UObjectToken.h"
#include "MapErrors.h"
#endif
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_Item_Log.h"
#include "Ticker.h"

#define LOCTEXT_NAMESPACE "ARPG_Human"

AHumanBase::AHumanBase(const FObjectInitializer& PCIP)
	:Super(PCIP)
{
	ARPG_MovementComponent->MovementState.bCanCrouch = true;
	ARPG_MovementComponent->SetIsReplicated(true);

	EnterReleaseStateAction = CreateDefaultSubobject<UHuman_EnterReleaseState>(GET_MEMBER_NAME_CHECKED(AHumanBase, EnterReleaseStateAction));

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AHumanBase, Head));
	{
		Head->SetupAttachment(GetMesh(), TEXT("head_root"));
	}
}

void AHumanBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (!bPreviewIsInited && !GetWorld()->IsGameWorld())
	{
		bPreviewIsInited = true;
		RefreshPreviewEquipedItem();
		ApplyCustomCharacterData();
	}
#endif

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->SetSubInstanceClassByTag(MoveAnimInstanceOverrideTagName, DefualtMoveAnimBp);
	}
}

void AHumanBase::PreSave(const class ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);

#if WITH_EDITOR
	EquipmentList.Empty();
#endif
}

void AHumanBase::PostDuplicate(EDuplicateMode::Type DuplicateMode)
{
	Super::PostDuplicate(DuplicateMode);

#if WITH_EDITOR
	EquipmentList.Empty();
	LeftWeapon = nullptr;
	RightWeapon = nullptr;
	Arrow = nullptr;
#endif
}

#if WITH_EDITOR
void AHumanBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, DefaultLeftWeapon))
	{
		RefreshPreviewEquipedItem();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, DefaultRightWeapon))
	{
		RefreshPreviewEquipedItem();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, DefaultArrow))
	{
		RefreshPreviewEquipedItem();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, DefaultEquipmentList))
	{
		RefreshPreviewEquipedItem();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, CustomCharacterBodyData))
	{
		OnRep_CustomCharacterBodyData();
		GetMesh()->InitAnim(false);	
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, CustomCharacterHeadData))
	{
		OnRep_CustomCharacterHeadData();
		Head->InitAnim(false);
	}
}

void AHumanBase::CheckForErrors()
{
	Super::CheckForErrors();

	for (int32 Idx = 0; Idx < DefaultEquipmentList.Num(); ++Idx)
	{
		const UARPG_EquipmentCoreBase* Item = DefaultEquipmentList[Idx];
		if (!Item)
		{
			FFormatNamedArguments Arguments;
			Arguments.Add(TEXT("ActorName"), FText::FromString(GetPathName()));
			Arguments.Add(TEXT("Idx"), FText::AsNumber(Idx));
			FMessageLog("MapCheck").Error()
				->AddToken(FUObjectToken::Create(this))
				->AddToken(FTextToken::Create(FText::Format(LOCTEXT("MapCheck_Message_DefalutEquip", "{ActorName} : 默认装备[{Idx}]存在错误"), Arguments)))
				->AddToken(FMapErrorToken::Create(FName("ItemError")));
		}
	}
}

#endif

void AHumanBase::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHumanBase, LeftWeapon);
	DOREPLIFETIME(AHumanBase, RightWeapon);
	DOREPLIFETIME(AHumanBase, Arrow);
	DOREPLIFETIME(AHumanBase, EquipmentList);

	DOREPLIFETIME(AHumanBase, UseWeaponState);
	DOREPLIFETIME(AHumanBase, CustomCharacterBodyData);
	DOREPLIFETIME(AHumanBase, CustomCharacterHeadData);
}

void AHumanBase::WhenGameInit_Implementation()
{
	Super::WhenGameInit_Implementation();

	if (DefaultLeftWeapon)
	{
		UseItemImmediately(DefaultLeftWeapon, EUseItemInput::LeftMouse);
	}

	if (DefaultRightWeapon)
	{
		UseItemImmediately(DefaultRightWeapon, EUseItemInput::RightMouse);
	}

	if (DefaultArrow)
	{
		UseItemImmediately(DefaultArrow);
	}

	for (const UARPG_ItemCoreBase* DefaultEquipment : DefaultEquipmentList)
	{
		if (DefaultEquipment)
		{
			UseItemImmediately(DefaultEquipment);
		}
	}

	ApplyCustomCharacterData();
}

void AHumanBase::WhenPostLoad_Implementation()
{
	Super::WhenPostLoad_Implementation();

 	if (LeftWeapon)
 	{
 		OnRep_LeftWeapon(nullptr);
 	}
 	if (RightWeapon)
 	{
 		OnRep_RightWeapon(nullptr);
 	}
 	if (Arrow)
 	{
 		OnRep_Arrow(nullptr);
 		Arrow->AttachWeaponTo(GetMesh(), QuiverSocketName);
 	}
 	if (UseWeaponState == EUseWeaponState::NoneWeapon_Default)
 	{
 		LetTheWeaponInWeaponBack();
 	}
 	else
 	{
 		LetTheWeaponInHand();
 	}
 	OnRep_EquipmentList();
 
	ApplyCustomCharacterData();
}

TArray<UARPG_ItemCoreBase*> AHumanBase::GetInitItemList() const
{
	TArray<UARPG_ItemCoreBase*> Res = Super::GetInitItemList();
	if (DefaultLeftWeapon)
	{
		Res.Add(DefaultLeftWeapon);
	}
	if (DefaultRightWeapon)
	{
		Res.Add(DefaultRightWeapon);
	}
	if (DefaultArrow)
	{
		Res.Add(DefaultArrow);
	}
	for (UARPG_EquipmentCoreBase* DefaultEquipment : DefaultEquipmentList)
	{
		if (DefaultEquipment)
		{
			Res.Add(DefaultEquipment);
		}
	}
	return Res;
}

class AARPG_WeaponBase* AHumanBase::EquipWaepon_Implementation(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput)
{
	switch (WeaponCore->WeaponUseType)
	{
	case EWeaponUseType::BothHandForLeft:
		return UseBothHandWeaponImpl(EUseItemInput::LeftMouse, WeaponCore);
	case EWeaponUseType::SingleHandForLeft:
		return UseSingleWeaponImpl(EUseItemInput::LeftMouse, WeaponCore);
	case EWeaponUseType::BothHandForRight:
		return UseBothHandWeaponImpl(EUseItemInput::RightMouse, WeaponCore);
	case EWeaponUseType::SingleHandForRight:
		return UseSingleWeaponImpl(EUseItemInput::RightMouse, WeaponCore);
	case EWeaponUseType::BothHand:
		return UseBothHandWeaponImpl(UseItemInput, WeaponCore);
	case EWeaponUseType::SingleHand:
		return UseSingleWeaponImpl(UseItemInput, WeaponCore);
	default:
		check(0);
	}
	return nullptr;
}

class AARPG_WeaponBase* AHumanBase::UseBothHandWeaponImpl(EUseItemInput UseItemInput, class UARPG_WeaponCoreBase* WeaponCore)
{
	switch (UseItemInput)
	{
	case EUseItemInput::LeftMouse:
		if (RightWeapon)
		{
			SetRightWeapon(nullptr);
		}
		return EquipSingleLeftWeapon(WeaponCore);
	case EUseItemInput::RightMouse:
		if (LeftWeapon)
		{
			SetLeftWeapon(nullptr);
		}
		return EquipSingleRightWeapon(WeaponCore);
	}
	return nullptr;
}

class AARPG_WeaponBase* AHumanBase::UseSingleWeaponImpl(EUseItemInput UseItemInput, class UARPG_WeaponCoreBase* WeaponCore)
{
	switch (UseItemInput)
	{
	case EUseItemInput::LeftMouse:
		if (RightWeapon)
		{
			if (RightWeapon->IsBothHandWeapon())
			{
				SetRightWeapon(nullptr);
			}
			else if (RightWeapon->IsEqualWithItemCore(WeaponCore))
			{
				if (Inventory->GetItemNumber(RightWeapon) == 1)
				{
					SetRightWeapon(nullptr);
				}
			}
		}
		return EquipSingleLeftWeapon(WeaponCore);
	case EUseItemInput::RightMouse:
		if (LeftWeapon)
		{
			if (LeftWeapon->IsBothHandWeapon())
			{
				SetLeftWeapon(nullptr);
			}
			else if (LeftWeapon->IsEqualWithItemCore(WeaponCore))
			{
				if (Inventory->GetItemNumber(LeftWeapon) == 1)
				{
					SetLeftWeapon(nullptr);
				}
			}
		}
		return EquipSingleRightWeapon(WeaponCore);
	}
	return nullptr;
}

class AARPG_ArrowBase* AHumanBase::EquipArrow_Implementation(class UARPG_ArrowCoreBase* ArrowCore, EUseItemInput UseItemInput)
{
	if (Arrow && Arrow->IsEqualWithItemCore(ArrowCore))
	{
		SetArrow(nullptr);
		return nullptr;
	}
	else
	{
		AARPG_ArrowBase* CurArrow = Cast<AARPG_ArrowBase>(ArrowCore->SpawnItemActorForOwner(this, this));
		if (CurArrow)
		{
			SetArrow(CurArrow);
			CurArrow->AttachWeaponTo(GetMesh(), QuiverSocketName);
		}
		return CurArrow;
	}
}

class AARPG_EquipmentBase* AHumanBase::EquipEquipment_Implementation(class UARPG_EquipmentCoreBase* EquipmentCore, EUseItemInput UseItemInput)
{
	if (!EquipmentCore->EquipmentType)
	{
		Item_Display_LOG("%s的装备类型未设置", *UXD_DebugFunctionLibrary::GetDebugName(EquipmentCore));
		return nullptr;
	}

	AARPG_EquipmentBase* ReturnEquipment = nullptr;
	int32 FindIndex = EquipmentList.IndexOfByPredicate([EquipmentCore](AARPG_EquipmentBase* E_Equipment) {return E_Equipment->IsEqualWithItemCore(EquipmentCore); });
	if (FindIndex != INDEX_NONE)
	{
		EquipmentList.RemoveAt(FindIndex);
		ReturnEquipment = nullptr;
	}
	else
	{
		TArray<AARPG_EquipmentBase*> NeedRemoveEquipment;
		for (AARPG_EquipmentBase* E_Equipment : EquipmentList)
		{
			if (E_Equipment && (E_Equipment->GetItemCoreConst()->EquipmentType & EquipmentCore->EquipmentType))
			{
				NeedRemoveEquipment.Add(E_Equipment);
			}
		}
		for (AARPG_EquipmentBase* E_Equipment : NeedRemoveEquipment)
		{
			EquipmentList.Remove(E_Equipment);
		}

		ReturnEquipment = Cast<AARPG_EquipmentBase>(EquipmentCore->SpawnItemActorForOwner(this, this));

		EquipmentList.Add(ReturnEquipment);
	}

	OnRep_EquipmentList();

	return ReturnEquipment;
}

bool AHumanBase::IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const
{
	return Super::IsDefenseSucceed_Implementation(DamageFromLocation, HitInfo) && UKismetMathLibrary::InRange_FloatFloat((UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageFromLocation) - GetActorRotation()).GetNormalized().Yaw, -90.f, 90.f);
}

bool AHumanBase::IsInReleaseState() const
{
	return UseWeaponState == EUseWeaponState::NoneWeapon_Default;
}

bool AHumanBase::CanAttack_Implementation(class AActor* AttackTarget) const
{
	return true;
}

FVector AHumanBase::GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const
{
	if (RightWeapon)
	{
		return IARPG_AI_BattleInterface::GetAttackMoveLocation(RightWeapon, AttackTarget);
	}
	else if (LeftWeapon)
	{
		return IARPG_AI_BattleInterface::GetAttackMoveLocation(LeftWeapon, AttackTarget);
	}
	else
	{
		return AttackTarget->GetActorLocation();
	}
}

FRotator AHumanBase::GetAttackFaceRotation_Implementation(class AActor* AttackTarget) const
{
	if (RightWeapon)
	{
		return IARPG_AI_BattleInterface::GetAttackFaceRotation(RightWeapon, AttackTarget);
	}
	else if (LeftWeapon)
	{
		return IARPG_AI_BattleInterface::GetAttackFaceRotation(LeftWeapon, AttackTarget);
	}
	else
	{
		return (AttackTarget->GetActorLocation() - GetActorLocation()).Rotation();
	}
}

bool AHumanBase::IsAllowedAttack_Implementation(class AActor* AttackTarget) const
{
	if (RightWeapon)
	{
		return IARPG_AI_BattleInterface::IsAllowedAttack(RightWeapon, AttackTarget);
	}
	else if (LeftWeapon)
	{
		return IARPG_AI_BattleInterface::IsAllowedAttack(LeftWeapon, AttackTarget);
	}
	else
	{
		return (AttackTarget->GetActorLocation() - GetActorLocation()).Size() < 200.f;
	}
}

void AHumanBase::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	if (RightWeapon)
	{
		return IARPG_AI_BattleInterface::InvokeAttack(RightWeapon, AttackTarget, OnAttackFinished);
	}
	else if (LeftWeapon)
	{
		return IARPG_AI_BattleInterface::InvokeAttack(LeftWeapon, AttackTarget, OnAttackFinished);
	}
	else
	{
		ARPG_InputPressed(EARPG_InputType::RightLightAttack);
		GetWorld()->GetTimerManager().SetTimer(FinishAttack_TimeHandle, FTimerDelegate::CreateUObject(this, &AHumanBase::FinishAttack, OnAttackFinished), 0.2f, false);
	}
}

void AHumanBase::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{
	if (RightWeapon)
	{
		return IARPG_AI_BattleInterface::AbortAttack(RightWeapon, AttackTarget, OnAttackAborted);
	}
	else if (LeftWeapon)
	{
		return IARPG_AI_BattleInterface::AbortAttack(LeftWeapon, AttackTarget, OnAttackAborted);
	}
	else
	{
		ARPG_InputReleased(EARPG_InputType::RightLightAttack);
		OnAttackAborted.ExecuteIfBound();
	}
}

void AHumanBase::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{
	if (RightWeapon)
	{
		return IARPG_AI_BattleInterface::AttackingTick(RightWeapon, AttackTarget, DeltaSecond);
	}
	else if (LeftWeapon)
	{
		return IARPG_AI_BattleInterface::AttackingTick(LeftWeapon, AttackTarget, DeltaSecond);
	}
}

void AHumanBase::FinishAttack(FBP_OnAttackFinished OnAttackFinished)
{
	ARPG_InputReleased(EARPG_InputType::RightLightAttack);
	OnAttackFinished.ExecuteIfBound(true);
}

void AHumanBase::WhenTakeBackWeaponFinished(UAnimMontage* AnimMontage, bool bInterrupted, FOnCharacterBehaviorFinished OnBehaviorFinished)
{
	if (FOnMontageBlendingOutStarted* OnMontageBlendingOutStarted = GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate(AnimMontage))
	{
		OnMontageBlendingOutStarted->Unbind();
	}
	OnBehaviorFinished.ExecuteIfBound(bInterrupted == false);
}

void AHumanBase::SetMoveAnimInstanceOverride(TSubclassOf<UAnimInstance> MoveAnimInstanceOverride)
{
	GetMesh()->GetAnimInstance()->SetSubInstanceClassByTag(MoveAnimInstanceOverrideTagName, MoveAnimInstanceOverride ? MoveAnimInstanceOverride : DefualtMoveAnimBp);
}

void AHumanBase::OnRep_CustomCharacterBodyData()
{
	CustomCharacterBodyData.ApplyMorphTarget(GetMesh());
	CustomCharacterBodyData.ApplyAllMaterialData(GetMesh());
}

void AHumanBase::OnRep_CustomCharacterHeadData()
{
	CustomCharacterHeadData.ApplyMorphTarget(Head);
	CustomCharacterHeadData.ApplyAllMaterialData(Head);
}

void AHumanBase::SetHideShorts(bool Hide)
{
#if DO_CHECK
	if (GetWorld()->IsGameWorld())
	{
		if (!((Hide && HideShortsCounter < 7) || (!Hide && HideShortsCounter > 0)))
		{
			check(0);
			return;
		}
	}
#endif

	HideShortsCounter += Hide ? 1 : -1;
	if (HideShortsCounter <= 1)
	{
		WhenHideShortStateChange(IsHideShorts());
	}
}

void AHumanBase::SetHideUnderwear(bool Hide)
{
#if DO_CHECK
	if (GetWorld()->IsGameWorld())
	{
		if (!((Hide && HideUnderwearCounter < 7) || (!Hide && HideUnderwearCounter > 0)))
		{
			check(0);
			return;
		}
	}
#endif

	HideUnderwearCounter += Hide ? 1 : -1;
	if (HideUnderwearCounter <= 1)
	{
		WhenHideUnderwearStateChange(IsHideUnderwear());
	}
}

void AHumanBase::NetForceSyncCustomCharacterData()
{
	SyncCustomCharacterData_ToServer(CustomCharacterBodyData, CustomCharacterHeadData);
}

void AHumanBase::ApplyCustomCharacterData()
{
	OnRep_CustomCharacterBodyData();
	OnRep_CustomCharacterHeadData();
}

void AHumanBase::SyncCustomCharacterData_ToServer_Implementation(const FCustomCharacterRuntimeData& SyncCustomCharacterBodyData, const FCustomCharacterRuntimeData& SyncCustomCharacterHeadData)
{
	CustomCharacterBodyData = SyncCustomCharacterBodyData;
	CustomCharacterHeadData = SyncCustomCharacterHeadData;
}

bool AHumanBase::SyncCustomCharacterData_ToServer_Validate(const FCustomCharacterRuntimeData& SyncCustomCharacterBodyData, const FCustomCharacterRuntimeData& SyncCustomCharacterHeadData)
{
	return true;
}

void AHumanBase::OnRep_UseWeaponState()
{
	ARPG_MovementComponent->bAiming = (UseWeaponState != EUseWeaponState::NoneWeapon_Default);
}

void AHumanBase::SetUseWeaponState(EUseWeaponState NewUseWeaponState)
{
	UseWeaponState = NewUseWeaponState;
	OnRep_UseWeaponState();
}

void AHumanBase::OnRep_EquipVariable(class AARPG_ItemBase* CurEquip, class AARPG_ItemBase* PreEquip)
{
	if (PreEquip)
	{
		OnRep_NotEquipItemImpl(PreEquip);
	}
	if (CurEquip)
	{
		OnRep_EquipItemImpl(CurEquip);
	}
	return;
}

void AHumanBase::OnRep_EquipItemImpl(class AARPG_ItemBase* CurEquip)
{
	CurEquip->WhenUse(this);
	WhenEquip(CurEquip);
	OnEquip.Broadcast(this, CurEquip);
}

void AHumanBase::OnRep_NotEquipItemImpl(class AARPG_ItemBase* PreEquip)
{
#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld())
	{
		PreEquip->Destroy();
	}
	else
#endif
	{
		PreEquip->SetLifeSpan(1.f);
		PreEquip->SetActorHiddenInGame(true);
	}

	if (PreEquip->ItemCore)
	{
		PreEquip->WhenNotUse(this);
		WhenNotEquip(PreEquip);
		OnNotEquip.Broadcast(this, PreEquip);
	}
}

void AHumanBase::SetLeftWeapon(class AARPG_WeaponBase* ToLeftWeapon)
{
	SetEquipVariable(LeftWeapon, ToLeftWeapon);
	if (LeftWeapon == nullptr && RightWeapon == nullptr)
	{
		SetUseWeaponState(EUseWeaponState::NoneWeapon_Default);
	}
}

void AHumanBase::OnRep_LeftWeapon(class AARPG_WeaponBase* PreLeftWeapon)
{
	if (LeftWeapon)
	{
		auto WhenItemCoreValid = [=]()
		{
			bIsLeftWeaponInWeaponBack = LeftWeapon->GetAttachParentSocketName() == LeftWeapon->GetItemCoreConst()->RightWeaponInWeaponBackSocket;
			OnRep_EquipVariable(LeftWeapon, PreLeftWeapon);
		};

		if (LeftWeapon->ItemCore)
		{
			WhenItemCoreValid();
		}
		else
		{
			LeftWeapon->OnItemCoreValidNative.BindWeakLambda(this, WhenItemCoreValid);
		}
	}
	else
	{
		bIsLeftWeaponInWeaponBack = false;
		OnRep_EquipVariable(LeftWeapon, PreLeftWeapon);
	}
}

void AHumanBase::SetRightWeapon(class AARPG_WeaponBase* ToRightWeapon)
{
	SetEquipVariable(RightWeapon, ToRightWeapon);
	if (LeftWeapon == nullptr && RightWeapon == nullptr)
	{
		SetUseWeaponState(EUseWeaponState::NoneWeapon_Default);
	}
}

void AHumanBase::OnRep_RightWeapon(class AARPG_WeaponBase* PreRightWeapon)
{
	if (RightWeapon)
	{
		auto WhenItemCoreValid = [=]()
		{
			bIsRightWeaponInWeaponBack = RightWeapon->GetAttachParentSocketName() == RightWeapon->GetItemCoreConst()->RightWeaponInWeaponBackSocket;
			OnRep_EquipVariable(RightWeapon, PreRightWeapon);
		};

		if (RightWeapon->ItemCore)
		{
			WhenItemCoreValid();
		}
		else
		{
			RightWeapon->OnItemCoreValidNative.BindWeakLambda(this, WhenItemCoreValid);
		}
	}
	else
	{
		bIsRightWeaponInWeaponBack = false;
		OnRep_EquipVariable(RightWeapon, PreRightWeapon);
	}
}

class AARPG_WeaponBase* AHumanBase::EquipSingleRightWeapon(class UARPG_ItemCoreBase* WeaponCore)
{
	if (RightWeapon)
	{
		if (RightWeapon->IsEqualWithItemCore(WeaponCore))
		{
			SetRightWeapon(nullptr);
			if (LeftWeapon == nullptr)
			{
				SetUseWeaponState(EUseWeaponState::NoneWeapon_Default);
			}
			return nullptr;
		}
		else
		{
			//把原来手上的武器卸下
			SetRightWeapon(nullptr);
		}
	}
	if (LeftWeapon)
	{
		switch (LeftWeapon->GetItemCoreConst()->WeaponUseType)
		{
			//卸下左手双手武器
		case EWeaponUseType::BothHandForLeft:
			SetLeftWeapon(nullptr);
		}
	}
	//Spawn武器并装备
	SetRightWeapon(Cast<AARPG_WeaponBase>(WeaponCore->SpawnItemActorForOwner(this, this)));
	if (RightWeapon)
	{
		RightWeaponInWeaponBack();
		if (UseWeaponState != EUseWeaponState::NoneWeapon_Default)
		{
			PlayMontage(PullOutWeaponMontage, {});
		}
	}

	return RightWeapon;
}

class AARPG_WeaponBase* AHumanBase::EquipSingleLeftWeapon(class UARPG_ItemCoreBase* WeaponCore)
{
	if (LeftWeapon)
	{
		if (LeftWeapon->IsEqualWithItemCore(WeaponCore))
		{
			SetLeftWeapon(nullptr);
			if (RightWeapon == nullptr)
			{
				SetUseWeaponState(EUseWeaponState::NoneWeapon_Default);
			}
			return nullptr;
		}
		else
		{
			//把原来手上的武器卸下
			SetLeftWeapon(nullptr);
		}
	}
	if (RightWeapon)
	{
		switch (RightWeapon->GetItemCoreConst()->WeaponUseType)
		{
			//卸下右手双手武器
		case EWeaponUseType::BothHandForRight:
			SetRightWeapon(nullptr);
		}
	}
	//Spawn武器并装备
	SetLeftWeapon(Cast<AARPG_WeaponBase>(WeaponCore->SpawnItemActorForOwner(this, this)));
	if (LeftWeapon)
	{
		LeftWeaponInWeaponBack();
		if (UseWeaponState != EUseWeaponState::NoneWeapon_Default)
		{
			PlayMontage(PullOutWeaponMontage, {});
		}
	}

	return LeftWeapon;
}

void AHumanBase::RightWeaponInHand()
{
	bIsRightWeaponInWeaponBack = false;
	RightWeapon->AttachWeaponTo(GetMesh(), RightWeapon->GetItemCoreConst()->RightWeaponInHandSocket);
	RightWeapon->WhenInHand();
}

void AHumanBase::LeftWeaponInHand()
{
	bIsLeftWeaponInWeaponBack = false;
	LeftWeapon->AttachWeaponTo(GetMesh(), LeftWeapon->GetItemCoreConst()->LeftWeaponInHandSocket);
	LeftWeapon->WhenInHand();
}

void AHumanBase::RightWeaponInWeaponBack()
{
	bIsRightWeaponInWeaponBack = true;
	RightWeapon->AttachWeaponTo(GetMesh(), RightWeapon->GetItemCoreConst()->RightWeaponInWeaponBackSocket);
	RightWeapon->WhenInWeaponBack();
}

void AHumanBase::LeftWeaponInWeaponBack()
{
	bIsLeftWeaponInWeaponBack = true;
	LeftWeapon->AttachWeaponTo(GetMesh(), LeftWeapon->GetItemCoreConst()->LeftWeaponInWeaponBackSocket);
	LeftWeapon->WhenInWeaponBack();
}

void AHumanBase::LetTheWeaponInHand()
{
	SetUseWeaponState(EUseWeaponState::UsingWeapon_Defalut);
	if (LeftWeapon)
	{
		LeftWeaponInHand();
	}
	if (RightWeapon)
	{
		RightWeaponInHand();
	}
}

void AHumanBase::LetTheWeaponInWeaponBack()
{
	SetUseWeaponState(EUseWeaponState::NoneWeapon_Default);
	if (LeftWeapon)
	{
		LeftWeaponInWeaponBack();
	}
	if (RightWeapon)
	{
		RightWeaponInWeaponBack();
	}
}

void AHumanBase::InvokePullOutWeapon()
{
	if (LeftWeapon || RightWeapon)
	{
		if (UseWeaponState == EUseWeaponState::NoneWeapon_Default && CanPlayFullBodyMontage())
		{
			PlayMontage(PullOutWeaponMontage, {});
		}
	}
}

void AHumanBase::InvokeTakeBackWeapon()
{
	if (UseWeaponState == EUseWeaponState::UsingWeapon_Defalut && CanPlayFullBodyMontage())
	{
		PlayMontage(TakeBackWeaponMontage, {});
	}
}

void AHumanBase::SetArrow(class AARPG_ArrowBase* ToArrow)
{
	SetEquipVariable(Arrow, ToArrow);
}

void AHumanBase::OnRep_Arrow(class AARPG_ArrowBase* PreArrow)
{
	if (Arrow)
	{
		auto WhenItemCoreValid = [=]()
		{
			OnRep_EquipVariable(Arrow, PreArrow);
		};
		if (Arrow->ItemCore)
		{
			WhenItemCoreValid();
		}
		else
		{
			Arrow->OnItemCoreValidNative.BindWeakLambda(this, WhenItemCoreValid);
		}
	}
	else
	{
		OnRep_EquipVariable(Arrow, PreArrow);
	}
}

void AHumanBase::OnRep_EquipmentList()
{
	//找出之前的
	for (AARPG_EquipmentBase* Equipment : TSet<AARPG_EquipmentBase*>(PreEquipmentList).Difference(TSet<AARPG_EquipmentBase*>(EquipmentList)))
	{
		if (Equipment)
		{
			OnRep_NotEquipItemImpl(Equipment);
		}
	}
	//找出现在的
	for (AARPG_EquipmentBase* Equipment : TSet<AARPG_EquipmentBase*>(EquipmentList).Difference(TSet<AARPG_EquipmentBase*>(PreEquipmentList)))
	{
		if (Equipment)
		{
			if (Equipment->ItemCore)
			{
				OnRep_EquipItemImpl(Equipment);
			}
			else
			{
				Equipment->OnItemCoreValidNative.BindWeakLambda(this, [=]()
					{
						OnRep_EquipItemImpl(Equipment);
					});
			}
		}
	}
	PreEquipmentList = EquipmentList;
}

#if WITH_EDITOR
void AHumanBase::RefreshPreviewEquipedItem()
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}		
	
	FEditorScriptExecutionGuard ScriptGuard;

	if (LeftWeapon)
	{
		EquipWaepon(LeftWeapon->GetItemCoreConst(), EUseItemInput::LeftMouse);
	}
	if (DefaultLeftWeapon)
	{
		EquipWaepon(DefaultLeftWeapon, EUseItemInput::LeftMouse);
		if (LeftWeapon)
		{
			LeftWeapon->SetFlags(RF_Transient);
		}
	}
	if (RightWeapon)
	{
		EquipWaepon(RightWeapon->GetItemCoreConst(), EUseItemInput::RightMouse);
	}
	if (DefaultRightWeapon)
	{
		EquipWaepon(DefaultRightWeapon, EUseItemInput::RightMouse);
		if (RightWeapon)
		{
			RightWeapon->SetFlags(RF_Transient);
		}
	}
	if (Arrow)
	{
		EquipArrow(Arrow->GetItemCoreConst(), EUseItemInput::LeftMouse);
	}
	if (DefaultArrow)
	{
		EquipArrow(DefaultArrow, EUseItemInput::LeftMouse);
		if (Arrow)
		{
			Arrow->SetFlags(RF_Transient);
		}
	}

	//Save的时候会清零，这边要恢复
	EquipmentList = PreviewEquipmentList;
	for (AARPG_EquipmentBase* Equipment : PreviewEquipmentList)
	{
		if (Equipment)
		{
			EquipEquipment(Equipment->GetItemCoreConst(), EUseItemInput::LeftMouse);
		}
	}
	PreviewEquipmentList.Empty();

	for (UARPG_EquipmentCoreBase* Equipment : DefaultEquipmentList)
	{
		if (Equipment)
		{
			if (AARPG_EquipmentBase * CurEquipment = EquipEquipment(Equipment, EUseItemInput::LeftMouse))
			{
				CurEquipment->SetFlags(RF_Transient);
				PreviewEquipmentList.Add(CurEquipment);
			}
		}
	}
}
#endif

#undef LOCTEXT_NAMESPACE
