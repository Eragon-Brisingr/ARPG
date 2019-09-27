// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Abstract/ARPG_ItemBase.h"
#include "ARPG_EquipmentBase.generated.h"

class UARPG_EquipmentCoreBase;

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "装备实体"))
class ARPG_API AARPG_EquipmentBase : public AARPG_ItemBase
{
	GENERATED_BODY()
public:
	AARPG_EquipmentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	//蓝图编译时会替换Master会丢失，将Master置回
	void EditorReplacedActor(AActor* OldActor) override;
#endif
	void WhenUse(class ACharacterBase* ItemOwner) override;

	void WhenNotUse(class ACharacterBase* ItemOwner) override;
public:
	const UARPG_EquipmentCoreBase* GetItemCoreConst() const;
};

UCLASS()
class ARPG_API AARPG_Equipment_StaticMesh : public AARPG_EquipmentBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Equipment_StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void InitItemMesh() override { InitStaticMeshComponent(StaticMeshComponent); }
};

UCLASS()
class ARPG_API AARPG_Equipment_SkeletalMesh : public AARPG_EquipmentBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Equipment_SkeletalMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitItemMesh() override { InitSkeletalMeshComponent(SkeletalMeshComponent); }
};
