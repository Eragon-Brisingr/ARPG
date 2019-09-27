// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ItemBase.h"
#include "ItemTypeUtils.h"
#include "ARPG_InteractInterface.h"
#include "ARPG_ItemBase.generated.h"

class ACharacterBase;
class UXD_ItemCoreBase;
class UARPG_ItemCoreBase;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisplayName = "物品实体"))
class ARPG_API AARPG_ItemBase : public AXD_ItemBase, public IARPG_InteractInterface
{
	GENERATED_BODY()
public:
	//Begin IARPG_InteractInterface
	void WhenExecuteInteract_Implementation(ACharacterBase* InteractInvoker) override;
	bool CanInteract_Implementation(const ACharacterBase* InteractInvoker) const override;
	//End IARPG_InteractInterface

#if WITH_EDITOR
	//由ARPG_Editor模块发起，GEditor->OnObjectsReplaced()
	//蓝图编译时会替换Flag，将RF_Transient状态置回
	void EditorReplacedActor(AActor* OldActor) override;
#endif
public:
	AARPG_ItemBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetWeight() const;

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	float GetPrice() const;

	UFUNCTION(BlueprintPure, Category = "物品|基础")
	FText GetItemTypeDesc() const;
	
	UFUNCTION(BlueprintPure, Category = "物品|基础")
	FText GetDescribe() const;

	UFUNCTION(BlueprintCallable, Category = "物品|基础")
	ACharacterBase* GetItemOwner() const;

	UFUNCTION(BlueprintCallable, Category = "物品|基础")
	void SetItemOwner(ACharacterBase* ItemOwner);
public:
#if WITH_EDITORONLY_DATA
	// 用于确保WhenUse调用准确
	uint8 bIsWhenUseExecuted : 1;
#endif

	virtual void WhenUse(ACharacterBase* ItemOwner);
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "WhenUse"))
	void ReceiveWhenUse(ACharacterBase* ItemOwner);

	virtual void WhenNotUse(ACharacterBase* ItemOwner);
	UFUNCTION(BlueprintImplementableEvent, Category = "物品|基础", meta = (DisplayName = "WhenNotUse"))
	void ReceiveWhenNotUse(ACharacterBase* ItemOwner);
public:
	const UARPG_ItemCoreBase* GetItemCoreConst() const;
	UARPG_ItemCoreBase* GetItemCore() const;

#if WITH_EDITOR
	bool IsSelectable() const override;
#endif
};

UCLASS()
class ARPG_API AARPG_Item_StaticMesh : public AARPG_ItemBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Item_StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void InitItemMesh() override { InitStaticMeshComponent(StaticMeshComponent); }
};

UCLASS()
class ARPG_API AARPG_Item_SkeletalMesh : public AARPG_ItemBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Item_SkeletalMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitItemMesh() override { InitSkeletalMeshComponent(SkeletalMeshComponent); }
};
