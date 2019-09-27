// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/GameTask/Elements/ARPG_EventFlowElementBase.h"
#include "ItemTypeUtils.h"
#include "EFE_FindItem.generated.h"

class UXD_ItemCoreBase;
class AARPG_ItemBase;

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UEFE_FindItemBase : public UARPG_EventFlowElementBase
{
	GENERATED_BODY()
public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void WhenActivateEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	void WhenDeactiveEventFlowElement_Implementation(class APawn* EventFlowOwnerCharacter, class AController* EventFlowOwner) override;

	virtual bool IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const;

	UFUNCTION()
	void WhenAddItem(UXD_ItemCoreBase* ItemCore, int32 AddNumber, int32 ExistNumber);

	UFUNCTION()
	void WhenRemoveItem(UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "任务目标", Replicated, SaveGame, meta = (DisplayName = "目标数量", ClampMin = "1"))
	int32 TargetNumber = 1;

	UPROPERTY(BlueprintReadOnly, Category = "任务目标", Replicated, SaveGame, meta = (DisplayName = "当前数量"))
	int32 CurrentNumber;
};

UCLASS(meta = (DisplayName = "找到物品", Category = "物品相关"))
class ARPG_API UEFE_FindItem_ByRef : public UEFE_FindItemBase
{
	GENERATED_BODY()
public:
	UEFE_FindItem_ByRef();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void ReplicatedElementSubobject(bool& WroteSomething, class UActorChannel * Channel, class FOutBunch * Bunch, FReplicationFlags * RepFlags) override;

	bool IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const override;
	EEventFlowCompileMessageType GetCompileMessage_Implementation(FString& Message) const override;

	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "任务目标", Replicated, meta = (DisplayName = "目标物品"), SaveGame, Instanced)
	UARPG_ItemCoreBase* TargetItem;
};

UCLASS(meta = (DisplayName = "找到物品（类型）", Category = "物品相关"))
class ARPG_API UEFE_FindItem_ByType : public UEFE_FindItemBase
{
	GENERATED_BODY()
public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	bool IsNeedFindItem(UXD_ItemCoreBase* ItemCore) const override;
	EEventFlowCompileMessageType GetCompileMessage_Implementation(FString& Message) const override;
	FText ReceiveGetDescribe_Implementation() const override;

	UPROPERTY(EditAnywhere, Category = "任务目标", Replicated, meta = (DisplayName = "目标物品类型"), SaveGame)
	TSubclassOf<UARPG_ItemCoreBase> TargetItemType;
};
