// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "XD_ItemType.h"
#include "HumanType.h"
#include "HumanBase.generated.h"

/**
 * 
 */

UCLASS()
class ARPG_API AHumanBase : public ACharacterBase
{
	GENERATED_BODY()
public:
	AHumanBase(const FObjectInitializer& PCIP);
	
public:
	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty > & OutLifetimeProps) const override;

	virtual void WhenGameInit_Implementation() override;

	virtual TArray<struct FXD_Item> GetInitItemList() const override;

	virtual class AARPG_WeaponBase* EquipWaepon_Implementation(class UARPG_WeaponCoreBase* WeaponCore, EUseItemInput UseItemInput) override;

	virtual class AARPG_ArrowBase* EquipArrow_Implementation(class UARPG_ArrowCoreBase* ArrowCore, EUseItemInput UseItemInput) override;

	virtual class AARPG_EquipmentBase* EquipEquipment_Implementation(class UARPG_EquipmentCoreBase* EquipmentCore, EUseItemInput UseItemInput) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|配置|常用", meta = (DisplayName = "默认左手武器", ExposeOnSpawn = "True"), SaveGame)
	FXD_Item DefaultLeftWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|配置|常用", meta = (DisplayName = "默认右手武器", ExposeOnSpawn = "True"), SaveGame)
	FXD_Item DefaultRightWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|配置|常用", meta = (DisplayName = "默认箭", ExposeOnSpawn = "True"), SaveGame)
	FXD_Item DefaultArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "角色|配置|常用", meta = (DisplayName = "默认装备列表", ExposeOnSpawn = "True"), SaveGame)
	TArray<FXD_Item> DefaultEquipmentList;
	
	//装备相关
public:
	UPROPERTY(Replicated, SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "角色", AdvancedDisplay)
	EUseWeaponState UseWeaponState;

	template<typename EquipType>
	void SetEquipVariable(EquipType*& CurEquip, EquipType* ToEquip);

	void OnRep_EquipVariable(class AARPG_ItemBase* CurEquip, class AARPG_ItemBase* PreEquip);

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

	UPROPERTY(EditDefaultsOnly, Category = "角色|配置")
	FName QuiverSocketName = TEXT("Quiver");

	UPROPERTY(EditDefaultsOnly, Category = "角色|配置")
	FName LeftHandHoldingArrowSocketName = TEXT("HoldingArrow_l");

	UPROPERTY(EditDefaultsOnly, Category = "角色|配置")
	FName RightHandHoldingArrowSocketName = TEXT("HoldingArrow_r");

	UPROPERTY()
	TArray<class AARPG_EquipmentBase*> PreEquipmentList;
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentList, VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = "角色", SaveGame)
	TArray<class AARPG_EquipmentBase*> EquipmentList;
	UFUNCTION()
	void OnRep_EquipmentList();
};

template<typename EquipType>
void AHumanBase::SetEquipVariable(EquipType*& CurEquip, EquipType* ToEquip)
{
	EquipType* PreEquip = CurEquip;
	if (PreEquip)
	{
		PreEquip->SetLifeSpan(1.f);
		PreEquip->SetActorHiddenInGame(true);
	}
	CurEquip = ToEquip;
	OnRep_EquipVariable(CurEquip, PreEquip);
}
