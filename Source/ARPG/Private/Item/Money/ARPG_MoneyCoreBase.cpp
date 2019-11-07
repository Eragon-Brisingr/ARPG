// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_MoneyCoreBase.h"
#include "ARPG_MoneyBase.h"

#define LOCTEXT_NAMESPACE "ARPG_Item"

FText UARPG_MoneyCoreBase::GetItemTypeDesc() const
{
	return LOCTEXT("金钱", "金钱");
}

TSubclassOf<AXD_ItemBase> UARPG_MoneyCoreBase::GetStaticMeshActor() const
{
	return AARPG_Money_StaticMesh::StaticClass();
}

TSubclassOf<AXD_ItemBase> UARPG_MoneyCoreBase::GetSkeletalMeshActor() const
{
	return AARPG_Money_SkeletalMesh::StaticClass();
}

#undef LOCTEXT_NAMESPACE
