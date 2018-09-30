// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EnvironmentPreviewActor.h"
#include "ARPG_EnvironmentManager.h"




AARPG_EnvironmentPreviewActorBase::AARPG_EnvironmentPreviewActorBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_EnvironmentManager>(GET_MEMBER_NAME_CHECKED(AARPG_EnvironmentPreviewActorBase, PreviewEnvironmentManager)))
{

}
