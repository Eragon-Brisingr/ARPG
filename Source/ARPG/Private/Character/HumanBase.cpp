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

AHumanBase::AHumanBase(const FObjectInitializer& PCIP)
	:Super(PCIP)
{
	ARPG_MovementComponent->MovementState.bCanCrouch = true;
	ARPG_MovementComponent->SetIsReplicated(true);

	{
#if WITH_EDITOR
		DefaultLeftWeapon.bShowNumber = false;
		DefaultLeftWeapon.ShowItemType = AARPG_WeaponBase::StaticClass();
		DefaultRightWeapon.bShowNumber = false;
		DefaultRightWeapon.ShowItemType = AARPG_WeaponBase::StaticClass();

		DefaultArrow.ShowItemType = AARPG_ArrowBase::StaticClass();
#endif
	}

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
	}
#endif
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

	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
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
		for (FARPG_Item& Equipment : DefaultEquipmentList)
		{
			Equipment.bShowNumber = false;
			Equipment.ShowItemType = AARPG_EquipmentBase::StaticClass();
		}
		RefreshPreviewEquipedItem();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, CustomCharacterBodyData))
	{
		GetMesh()->InitAnim(false);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AHumanBase, CustomCharacterBodyData))
	{
		Head->InitAnim(false);
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

	if (UARPG_ItemCoreBase* WeaponCore = Cast<UARPG_ItemCoreBase>(DefaultLeftWeapon.ItemCore))
	{
		UseItemImmediately(WeaponCore, EUseItemInput::LeftMouse);
	}

	if (UARPG_ItemCoreBase* WeaponCore = Cast<UARPG_ItemCoreBase>(DefaultRightWeapon.ItemCore))
	{
		UseItemImmediately(WeaponCore, EUseItemInput::RightMouse);
	}

	if (UARPG_ItemCoreBase* ArrowCore = Cast<UARPG_ItemCoreBase>(DefaultArrow.ItemCore))
	{
		UseItemImmediately(ArrowCore);
	}

	for (const FARPG_Item& DefaultEquipment : DefaultEquipmentList)
	{
		if (UARPG_ItemCoreBase* EquipmentCore = Cast<UARPG_ItemCoreBase>(DefaultEquipment.ItemCore))
		{
			UseItemImmediately(EquipmentCore);
		}
	}
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
}

TArray<struct FARPG_Item> AHumanBase::GetInitItemList() const
{
	TArray<FARPG_Item> Res = Super::GetInitItemList();
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
	for (const FARPG_Item& DefaultEquipment : DefaultEquipmentList)
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
	const AARPG_WeaponBase* Weapon = WeaponCore->GetItemDefaultActor<AARPG_WeaponBase>();
	switch (Weapon->WeaponUseType)
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
		SetArrow(CurArrow);
		CurArrow->AttachWeaponTo(GetMesh(), QuiverSocketName);
		return CurArrow;
	}
}

class AARPG_EquipmentBase* AHumanBase::EquipEquipment_Implementation(class UARPG_EquipmentCoreBase* EquipmentCore, EUseItemInput UseItemInput)
{
	const AARPG_EquipmentBase* Equipment = EquipmentCore->GetItemDefaultActor<AARPG_EquipmentBase>();
	if (!Equipment->EquipmentType)
	{
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
			if (E_Equipment && E_Equipment->EquipmentType & Equipment->EquipmentType)
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

void AHumanBase::OnRep_CustomCharacterBodyData()
{
	CustomCharacterBodyData.ApplyMorphTarget(GetMesh());
}

void AHumanBase::OnRep_CustomCharacterHeadData()
{
	CustomCharacterHeadData.ApplyMorphTarget(Head);
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
		PreEquip->WhenNotUse(this);
		OnNotEquip.Broadcast(this, PreEquip);
	}
	if (CurEquip)
	{
		CurEquip->WhenUse(this);
		OnEquip.Broadcast(this, CurEquip);
	}
}

void AHumanBase::SetLeftWeapon(class AARPG_WeaponBase* ToLeftWeapon)
{
	SetEquipVariable(LeftWeapon, ToLeftWeapon);
}

void AHumanBase::OnRep_LeftWeapon(class AARPG_WeaponBase* PreLeftWeapon)
{
	OnRep_EquipVariable(LeftWeapon, PreLeftWeapon);
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
	OnRep_EquipVariable(RightWeapon, PreRightWeapon);
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
		switch (LeftWeapon->WeaponUseType)
		{
			//卸下左手双手武器
		case EWeaponUseType::BothHandForLeft:
			SetLeftWeapon(nullptr);
		}
	}
	//Spawn武器并装备
	SetRightWeapon(Cast<AARPG_WeaponBase>(WeaponCore->SpawnItemActorForOwner(this, this)));
	RightWeaponInWeaponBack();
	if (UseWeaponState != EUseWeaponState::NoneWeapon_Default)
	{
		PlayMontage(PullOutWeaponMontage, {});
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
		switch (RightWeapon->WeaponUseType)
		{
			//卸下右手双手武器
		case EWeaponUseType::BothHandForRight:
			SetRightWeapon(nullptr);
		}
	}
	//Spawn武器并装备
	SetLeftWeapon(Cast<AARPG_WeaponBase>(WeaponCore->SpawnItemActorForOwner(this, this)));
	LeftWeaponInWeaponBack();
	if (UseWeaponState != EUseWeaponState::NoneWeapon_Default)
	{
		PlayMontage(PullOutWeaponMontage, {});
	}

	return LeftWeapon;
}

void AHumanBase::RightWeaponInHand()
{
	RightWeapon->AttachWeaponTo(GetMesh(), RightWeapon->RightWeaponInHandSocket);
	RightWeapon->WhenInHand();
}

void AHumanBase::LeftWeaponInHand()
{
	LeftWeapon->AttachWeaponTo(GetMesh(), LeftWeapon->LeftWeaponInHandSocket);
	LeftWeapon->WhenInHand();
}

void AHumanBase::RightWeaponInWeaponBack()
{
	RightWeapon->AttachWeaponTo(GetMesh(), RightWeapon->RightWeaponInWeaponBackSocket);
	RightWeapon->WhenInWeaponBack();
}

void AHumanBase::LeftWeaponInWeaponBack()
{
	LeftWeapon->AttachWeaponTo(GetMesh(), LeftWeapon->LeftWeaponInWeaponBackSocket);
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
	OnRep_EquipVariable(Arrow, PreArrow);
}

void AHumanBase::OnRep_EquipmentList()
{
	//找出之前的
	for (AARPG_EquipmentBase* Equipment : TSet<AARPG_EquipmentBase*>(PreEquipmentList).Difference(TSet<AARPG_EquipmentBase*>(EquipmentList)))
	{
		if (Equipment)
		{
			Equipment->SetLifeSpan(1.f);
			Equipment->SetActorHiddenInGame(true);
			Equipment->WhenNotUse(this);
			OnNotEquip.Broadcast(this, Equipment);
		}
	}
	//找出现在的
	for (AARPG_EquipmentBase* Equipment : TSet<AARPG_EquipmentBase*>(EquipmentList).Difference(TSet<AARPG_EquipmentBase*>(PreEquipmentList)))
	{
		if (Equipment)
		{
			Equipment->WhenUse(this);
			OnEquip.Broadcast(this, Equipment);
		}
	}
	PreEquipmentList = EquipmentList;
}

#if WITH_EDITOR
void AHumanBase::RefreshPreviewEquipedItem()
{
	FEditorScriptExecutionGuard ScriptGuard;

	if (LeftWeapon)
	{
		EquipWaepon(LeftWeapon->GetItemCore(), EUseItemInput::LeftMouse);
	}
	if (DefaultLeftWeapon)
	{
		EquipWaepon(DefaultLeftWeapon.GetItemCore<UARPG_WeaponCoreBase>(), EUseItemInput::LeftMouse);
		if (LeftWeapon)
		{
			LeftWeapon->SetFlags(RF_Transient);
		}
	}
	if (RightWeapon)
	{
		EquipWaepon(RightWeapon->GetItemCore(), EUseItemInput::RightMouse);
	}
	if (DefaultRightWeapon)
	{
		EquipWaepon(DefaultRightWeapon.GetItemCore<UARPG_WeaponCoreBase>(), EUseItemInput::RightMouse);
		if (RightWeapon)
		{
			RightWeapon->SetFlags(RF_Transient);
		}
	}
	if (Arrow)
	{
		EquipArrow(Arrow->GetItemCore(), EUseItemInput::LeftMouse);
	}
	if (DefaultArrow)
	{
		EquipArrow(DefaultArrow.GetItemCore<UARPG_ArrowCoreBase>(), EUseItemInput::LeftMouse);
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
			EquipEquipment(Equipment->GetItemCore(), EUseItemInput::LeftMouse);
		}
	}
	PreviewEquipmentList.Empty();

	for (const FARPG_Item& Equipment : DefaultEquipmentList)
	{
		if (Equipment)
		{
			if (AARPG_EquipmentBase* CurEquipment = EquipEquipment(Equipment.GetItemCore<UARPG_EquipmentCoreBase>(), EUseItemInput::LeftMouse))
			{
				CurEquipment->SetFlags(RF_Transient);
				PreviewEquipmentList.Add(CurEquipment);
			}
		}
	}
}
#endif
