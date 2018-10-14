// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_BowBase.h"
#include "ARPG_BowCoreBase.h"




AARPG_BowBase::AARPG_BowBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_BowCoreBase>(GET_MEMBER_NAME_CHECKED(AARPG_BowBase, InnerItemCore)))
{

}
