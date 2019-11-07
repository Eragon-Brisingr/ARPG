// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_PropBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>
#include "..\..\..\Public\Item\Prop\ARPG_PropBase.h"

AARPG_PropBase::AARPG_PropBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

AARPG_Prop_StaticMesh::AARPG_Prop_StaticMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(GET_MEMBER_NAME_CHECKED(AARPG_Prop_StaticMesh, StaticMeshComponent));

	SetRootComponent(StaticMeshComponent);
}

AARPG_Prop_SkeletalMesh::AARPG_Prop_SkeletalMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(GET_MEMBER_NAME_CHECKED(AARPG_Prop_SkeletalMesh, SkeletalMeshComponent));

	SetRootComponent(SkeletalMeshComponent);
}
