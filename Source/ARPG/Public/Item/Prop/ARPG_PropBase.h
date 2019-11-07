// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Abstract/ARPG_ItemBase.h"
#include "ARPG_PropBase.generated.h"

/**
 * 
 */
UCLASS(abstract, meta = (DisplayName = "道具实体"))
class ARPG_API AARPG_PropBase : public AARPG_ItemBase
{
	GENERATED_BODY()
public:
	AARPG_PropBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

UCLASS()
class ARPG_API AARPG_Prop_StaticMesh : public AARPG_PropBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Prop_StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void InitItemMesh() override { InitStaticMeshComponent(StaticMeshComponent); }
};

UCLASS()
class ARPG_API AARPG_Prop_SkeletalMesh : public AARPG_PropBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Prop_SkeletalMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitItemMesh() override { InitSkeletalMeshComponent(SkeletalMeshComponent); }
};

