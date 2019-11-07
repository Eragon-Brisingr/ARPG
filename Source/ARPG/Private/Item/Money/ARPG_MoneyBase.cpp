// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_MoneyBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>

AARPG_Money_StaticMesh::AARPG_Money_StaticMesh(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AARPG_Money_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Money_SkeletalMesh::AARPG_Money_SkeletalMesh(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AARPG_Money_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}
