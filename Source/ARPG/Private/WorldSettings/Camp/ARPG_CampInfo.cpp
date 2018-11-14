// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_CampInfo.h"
#include "ARPG_CampRelationship.h"

UARPG_CampInfo::UARPG_CampInfo(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_CampRelationship>(GET_MEMBER_NAME_CHECKED(UXD_CampInfo, DefaultCampRelationship))
							.SetDefaultSubobjectClass<UARPG_CampRelationship>(GET_MEMBER_NAME_CHECKED(UXD_CampInfo, SelfCampRelationship)))
{

}
