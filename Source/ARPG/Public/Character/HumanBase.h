// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "HumanType.h"
#include "CustomSkeletonConfig.h"
#include "HumanBase.generated.h"

class UAnimInstance;
class USkeletalMeshComponent;
class UARPG_ItemCoreBase;
class AARPG_WeaponBase;
class AARPG_ArrowBase;
class AARPG_EquipmentBase;

/**
 * 
 */

UCLASS()
class ARPG_API AHumanBase : public ACharacterBase
{
	GENERATED_BODY()
public:
	AHumanBase(const FObjectInitializer& PCIP);
	
	void OnConstruction(const FTransform& Transform) override;

	void PreSave(const class ITargetPlatform* TargetPlatform) override;
	void PostDuplicate(EDuplicateMode::Type DuplicateMode) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void CheckForErrors() override;
#endif
public:
	void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

	void WhenGameInit_Implementation() override;

	void WhenPostLoad_Implementation() override;

	TArray<UARPG_ItemCoreBase*> GetInitItemList() const override;

	AARPG_WeaponBase* EquipWaepon_Implementation(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput) override;

	AARPG_WeaponBase* UseBothHandWeaponImpl(EUseItemInput UseItemInput, class UARPG_WeaponCoreBase* WeaponCore);

	AARPG_WeaponBase* UseSingleWeaponImpl(EUseItemInput UseItemInput, class UARPG_WeaponCoreBase* WeaponCore);

	AARPG_ArrowBase* EquipArrow_Implementation(class UARPG_ArrowCoreBase* ArrowCore, EUseItemInput UseItemInput) override;

	AARPG_EquipmentBase* EquipEquipment_Implementation(class UARPG_EquipmentCoreBase* EquipmentCore, EUseItemInput UseItemInput) override;

	bool IsDefenseSucceed_Implementation(const FVector& DamageFromLocation, const FHitResult& HitInfo) const override;

	bool IsInReleaseState() const override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "角色")
	USkeletalMeshComponent* Head;

public:
	//IARPG_AI_BattleInterface
	bool CanAttack_Implementation(class AActor* AttackTarget) const override;
	FVector GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const override;
	FRotator GetAttackFaceRotation_Implementation(class AActor* AttackTarget) const override;
	bool IsAllowedAttack_Implementation(class AActor* AttackTarget) const override;
	void InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished) override;
	void AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted) override;
	void AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond) override;
	//
	FTimerHandle FinishAttack_TimeHandle;
	void FinishAttack(FBP_OnAttackFinished OnAttackFinished);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "配置|常用", meta = (DisplayName = "默认左手武器", ExposeOnSpawn = "True"), SaveGame)
	UARPG_WeaponCoreBase* DefaultLeftWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "配置|常用", meta = (DisplayName = "默认右手武器", ExposeOnSpawn = "True"), SaveGame)
	UARPG_WeaponCoreBase* DefaultRightWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "配置|常用", meta = (DisplayName = "默认箭", ExposeOnSpawn = "True"), SaveGame)
	UARPG_ArrowCoreBase* DefaultArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "配置|常用", meta = (DisplayName = "默认装备列表", ExposeOnSpawn = "True"), SaveGame)
	TArray<UARPG_EquipmentCoreBase*> DefaultEquipmentList;
	
	//装备相关
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "角色", AdvancedDisplay, ReplicatedUsing = OnRep_UseWeaponState, SaveGame)
	EUseWeaponState UseWeaponState;
	UFUNCTION()
	void OnRep_UseWeaponState();
	void SetUseWeaponState(EUseWeaponState NewUseWeaponState);

private:
	template<typename EquipType>
	void SetEquipVariable(EquipType*& CurEquip, EquipType* ToEquip);
	void OnRep_EquipVariable(class AARPG_ItemBase* CurEquip, class AARPG_ItemBase* PreEquip);
	void OnRep_UseItemImpl(class AARPG_ItemBase* CurEquip);
	void OnRep_NotUseImpl(class AARPG_ItemBase* PreEquip);

public:
	UPROPERTY(ReplicatedUsing = OnRep_LeftWeapon, VisibleAnywhere, AdvancedDisplay, BlueprintReadWrite, BlueprintSetter = "SetLeftWeapon", Category = "角色", SaveGame)
	class AARPG_WeaponBase* LeftWeapon;
	UFUNCTION(BlueprintSetter)
	void SetLeftWeapon(class AARPG_WeaponBase* ToLeftWeapon);
	UFUNCTION()
	void OnRep_LeftWeapon(class AARPG_WeaponBase* PreLeftWeapon);

	UPROPERTY(ReplicatedUsing = OnRep_RightWeapon, VisibleAnywhere, AdvancedDisplay, BlueprintReadWrite, BlueprintSetter = "SetRightWeapon", Category = "角色", SaveGame)
	class AARPG_WeaponBase* RightWeapon;
	UFUNCTION(BlueprintSetter)
	void SetRightWeapon(class AARPG_WeaponBase* ToRightWeapon);
	UFUNCTION()
	void OnRep_RightWeapon(class AARPG_WeaponBase* PreRightWeapon);

	class AARPG_WeaponBase* EquipSingleRightWeapon(class UARPG_ItemCoreBase* WeaponCore);
	class AARPG_WeaponBase* EquipSingleLeftWeapon(class UARPG_ItemCoreBase* WeaponCore);

	void RightWeaponInHand();
	void LeftWeaponInHand();
	void RightWeaponInWeaponBack();
	void LeftWeaponInWeaponBack();

	UPROPERTY(BlueprintReadOnly, Category = "角色|武器")
	uint8 bIsLeftWeaponInWeaponBack : 1;
	UPROPERTY(BlueprintReadOnly, Category = "角色|武器")
	uint8 bIsRightWeaponInWeaponBack : 1;

	void LetTheWeaponInHand();
	void LetTheWeaponInWeaponBack();

	UPROPERTY(EditDefaultsOnly, Category = "角色|行为")
	class UAnimMontage* PullOutWeaponMontage;

	UPROPERTY(EditDefaultsOnly, Category = "角色|行为")
	class UAnimMontage* TakeBackWeaponMontage;

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void InvokePullOutWeapon();

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void InvokeTakeBackWeapon();

	UPROPERTY(ReplicatedUsing = OnRep_Arrow, VisibleAnywhere, AdvancedDisplay, BlueprintReadWrite, BlueprintSetter = "SetArrow", Category = "角色", SaveGame)
	class AARPG_ArrowBase* Arrow;
	UFUNCTION(BlueprintSetter)
	void SetArrow(class AARPG_ArrowBase* ToArrow);
	UFUNCTION()
	void OnRep_Arrow(class AARPG_ArrowBase* PreArrow);

	UPROPERTY(EditDefaultsOnly, Category = "配置")
	FName QuiverSocketName = TEXT("dorlach_01");

	UPROPERTY(EditDefaultsOnly, Category = "配置")
	FName LeftHandHoldingArrowSocketName = TEXT("HoldingArrow_l");

	UPROPERTY(EditDefaultsOnly, Category = "配置")
	FName RightHandHoldingArrowSocketName = TEXT("HoldingArrow_r");

	UPROPERTY(Transient)
	TArray<class AARPG_EquipmentBase*> PreEquipmentList;
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentList, VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "角色", SaveGame)
	TArray<class AARPG_EquipmentBase*> EquipmentList;
	UFUNCTION()
	void OnRep_EquipmentList();

private:
#if WITH_EDITORONLY_DATA
	uint8 bPreviewIsInited : 1;
	UPROPERTY(Transient)
	TArray<class AARPG_EquipmentBase*> PreviewEquipmentList;
	void RefreshPreviewEquipedItem();
#endif

private:
	void WhenTakeBackWeaponFinished(UAnimMontage* AnimMontage, bool bInterrupted, FOnCharacterBehaviorFinished OnBehaviorFinished);

	//动画
public:
	UPROPERTY(EditDefaultsOnly, Category = "角色|动画")
	TSubclassOf<UAnimInstance> DefualtMoveAnimBp;

	UPROPERTY(EditDefaultsOnly, Category = "角色|动画")
	FName MoveAnimInstanceOverrideTagName = TEXT("Move");

	void SetMoveAnimInstanceOverride(TSubclassOf<UAnimInstance> MoveAnimInstanceOverride);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "角色|动画")
	uint8 bEnableHeadLookAt : 1;
	//角色定制
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "配置|定制", ReplicatedUsing = OnRep_CustomCharacterBodyData, SaveGame)
	FCustomCharacterRuntimeData CustomCharacterBodyData;
	UFUNCTION()
	void OnRep_CustomCharacterBodyData();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "配置|定制", ReplicatedUsing = OnRep_CustomCharacterHeadData, SaveGame)
	FCustomCharacterRuntimeData CustomCharacterHeadData;
	UFUNCTION()
	void OnRep_CustomCharacterHeadData();

	uint8 HideShortsCounter : 4;
	uint8 HideUnderwearCounter : 4;

	bool IsHideShorts() const { return static_cast<bool>(HideShortsCounter); }
	void SetHideShorts(bool Hide);
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|定制")
	void WhenHideShortStateChange(bool Hide);
	bool IsHideUnderwear() const { return static_cast<bool>(HideUnderwearCounter); }
	void SetHideUnderwear(bool Hide);
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|定制")
	void WhenHideUnderwearStateChange(bool Hide);

	UFUNCTION(BlueprintCallable, Category = "角色|定制")
	void NetForceSyncCustomCharacterData();
	UFUNCTION(Reliable, Server, WithValidation)
	void SyncCustomCharacterData_ToServer(const FCustomCharacterRuntimeData& SyncCustomCharacterBodyData, const FCustomCharacterRuntimeData& SyncCustomCharacterHeadData);

	void ApplyCustomCharacterData();
};

template<typename EquipType>
void AHumanBase::SetEquipVariable(EquipType*& CurEquip, EquipType* ToEquip)
{
	EquipType* PreEquip = CurEquip;
	CurEquip = ToEquip;
	OnRep_EquipVariable(CurEquip, PreEquip);
}
