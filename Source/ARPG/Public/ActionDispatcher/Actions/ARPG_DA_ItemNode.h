// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_DispatchableActionBase.h"
#include "ARPG_DA_ItemNode.generated.h"

class ACharacterBase;
class UXD_ItemCoreBase;
class UARPG_ItemCoreBase;

/**
 * 
 */
UCLASS(meta = (DisplayName = "移动物品"))
class ARPG_API UARPG_DA_MoveItem : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<ACharacterBase> Role;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<AActor> Other;

	UPROPERTY(SaveGame, meta = (DisplayName = "当移动道具结束"))
	FOnDispatchableActionFinishedEvent WhenMoveItemFinished;
};

UCLASS(meta = (DisplayName = "移动物品_记录变化"))
class ARPG_API UARPG_DA_MoveItem_Record : public UARPG_DA_MoveItem
{
	GENERATED_BODY()
public:
	void WhenActionActived() override;
	void WhenActionDeactived() override;
public:
	UPROPERTY(BlueprintReadOnly, Category = "物品", SaveGame)
	TArray<UARPG_ItemCoreBase*> RoleAddedItems;

	UPROPERTY(BlueprintReadOnly, Category = "物品", SaveGame)
	TArray<UARPG_ItemCoreBase*> RoleRemovedItems;

	UFUNCTION()
	void WhenAddItem(UXD_ItemCoreBase* ItemCore, int32 AddNumber, int32 ExistNumber);

	UFUNCTION()
	void WhenRemoveItem(UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber);
};

UCLASS(meta = (DisplayName = "交易物品"))
class ARPG_API UARPG_DA_TradeItem : public UXD_DispatchableActionBase
{
	GENERATED_BODY()
public:
	TArray<AActor*> GetAllRegistableEntities() const override;
	void WhenActionActived() override;
	void WhenActionDeactived() override;
	void WhenActionFinished() override;
public:
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<ACharacterBase> Role;
	UPROPERTY(SaveGame, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TSoftObjectPtr<AActor> Other;

	UPROPERTY(SaveGame, meta = (DisplayName = "当交易道具结束"))
	FOnDispatchableActionFinishedEvent WhenTradeItemFinished;
};
