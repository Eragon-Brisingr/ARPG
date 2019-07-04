// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Abstract/ARPG_ItemBase.h"
#include "ARPG_EquipmentBase.generated.h"

class UARPG_EquipmentCoreBase;

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
class ARPG_API AARPG_EquipmentBase : public AARPG_ItemBase
{
	GENERATED_BODY()
public:
	AARPG_EquipmentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const override;

	virtual FText GetItemTypeDescImpl_Implementation(const class UXD_ItemCoreBase* ItemCore) const override;

	virtual void WhenUse(class ACharacterBase* ItemOwner) override;

	virtual void WhenNotUse(class ACharacterBase* ItemOwner) override;

	virtual void WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "物品|装备", meta = (DisplayName = "装备插槽名"))
	FName EquipSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "物品|装备", meta = (DisplayName = "装备类型", Bitmask, BitmaskEnum = "EEquipmentType"))
	int32 EquipmentType;

public:
	const UARPG_EquipmentCoreBase* GetItemCore() const;
};
