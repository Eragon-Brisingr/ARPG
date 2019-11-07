// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Abstract/ARPG_ItemCoreBase.h"
#include "ARPG_EquipmentCoreBase.generated.h"

class UARPG_CharacterState_BuffBase;

/**
 * 
 */
UENUM(BlueprintType, meta = (Bitflags))
enum EEquipmentType
{
	Equipment_Head UMETA(DisplayName = "头部"),
	Equipment_UpperBody UMETA(DisplayName = "上身"),
	Equipment_LowerBody UMETA(DisplayName = "下身"),
	Equipment_Foot UMETA(DisplayName = "足部"),
	Equipment_Hand UMETA(DisplayName = "手臂"),
	Equipment_Cloak UMETA(DisplayName = "披风"),
	Equipment_Light UMETA(DisplayName = "灯")
};
ENUM_CLASS_FLAGS(EEquipmentType);

UCLASS(abstract, meta = (DisplayName = "装备"))
class ARPG_API UARPG_EquipmentCoreBase : public UARPG_ItemCoreBase
{
	GENERATED_BODY()

public:
	FText GetItemTypeDesc() const override;
	void UseItem(ACharacterBase* ItemOwner, EUseItemInput UseItemInput) override;
	void WhenRemoveFromInventory(class AActor* ItemOwner, int32 RemoveNumber, int32 ExistNumber) override;
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备", meta = (DisplayName = "装备插槽名"))
	FName EquipSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备", meta = (DisplayName = "装备类型", Bitmask, BitmaskEnum = "EEquipmentType"))
	int32 EquipmentType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备", meta = (DisplayName = "隐藏内裤"))
	uint8 bHideShorts : 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备", meta = (DisplayName = "隐藏内衣"))
	uint8 bHideUnderwear : 1;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "装备", meta = (DisplayName = "启用的Buff"))
	TArray<TSubclassOf<UARPG_CharacterState_BuffBase>> EnableBuffes;

	void WhenUse(ACharacterBase* ItemOwner) override;
	void WhenNotUse(ACharacterBase* ItemOwner) override;
};
