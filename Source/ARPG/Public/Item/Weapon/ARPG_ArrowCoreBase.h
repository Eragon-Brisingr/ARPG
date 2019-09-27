// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/ARPG_WeaponCoreBase.h"
#include "ARPG_ArrowCoreBase.generated.h"

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "箭"))
class ARPG_API UARPG_ArrowCoreBase : public UARPG_WeaponCoreBase
{
	GENERATED_BODY()
public:
	FText GetItemTypeDesc_Implementation() const override;
	void UseItem_Implementation(ACharacterBase* ItemOwner, EUseItemInput UseItemInput) override;
	void WhenRemoveFromInventory(class AActor* ItemOwner, int32 RemoveNumber, int32 ExistNumber) override;
	TSubclassOf<AXD_ItemBase> GetStaticMeshActor() const override;
	TSubclassOf<AXD_ItemBase> GetSkeletalMeshActor() const override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "武器", meta = (DisplayName = "箭袋"))
	TSoftObjectPtr<UStaticMesh> Dorlach;
};
