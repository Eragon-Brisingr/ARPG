// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_TimeManagerPreviewActor.h"
#include "ARPG_TimeManager.h"




AARPG_TimeManagerPreviewActorBase::AARPG_TimeManagerPreviewActorBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UARPG_TimeManager>(GET_MEMBER_NAME_CHECKED(AARPG_TimeManagerPreviewActorBase, PreviewTimeManager)))
{

}
