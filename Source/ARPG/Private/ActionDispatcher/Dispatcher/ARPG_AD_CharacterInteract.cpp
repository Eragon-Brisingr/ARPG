// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_AD_CharacterInteract.h"
#include "CharacterBase.h"

UARPG_AD_CharacterInteract::UARPG_AD_CharacterInteract()
{
	bIsMainDispatcher = true;
}

void UARPG_AD_CharacterInteract::WhenActived()
{
	if (bFaceToInteracter)
	{
		StartFaceToInteracter();
	}
}

void UARPG_AD_CharacterInteract::WhenDeactived(bool IsFinsihedCompleted)
{
	if (bFaceToInteracter)
	{
		StopFaceToInteracter();
	}
	CurrentActions.Empty();
}

void UARPG_AD_CharacterInteract::StartFaceToInteracter()
{
	bFaceToInteracter = true;
	Character->SetLockedTarget(InteractTarget.Get());
}

void UARPG_AD_CharacterInteract::StopFaceToInteracter()
{
	bFaceToInteracter = false;
	Character->SetLockedTarget(nullptr);
}
