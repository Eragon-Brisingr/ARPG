// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BowCoreBase.h"
#include "ARPG_BowBase.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

FText UARPG_BowCoreBase::GetItemTypeDesc_Implementation() const
{
	return LOCTEXT("ARPG_BowType", "弓");
}

TSubclassOf<AXD_ItemBase> UARPG_BowCoreBase::GetStaticMeshActor() const
{
	return AARPG_Bow_StaticMesh::StaticClass();
}

TSubclassOf<AXD_ItemBase> UARPG_BowCoreBase::GetSkeletalMeshActor() const
{
	return AARPG_Bow_SkeletalMesh::StaticClass();
}

#undef LOCTEXT_NAMESPACE
