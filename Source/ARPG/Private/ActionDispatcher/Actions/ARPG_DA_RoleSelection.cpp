// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_DA_RoleSelection.h"
#include "GameFramework/Pawn.h"
#include "CharacterBase.h"



UARPG_DA_RoleSelection_Interact::UARPG_DA_RoleSelection_Interact()
{
#if WITH_EDITORONLY_DATA
	bIsPluginAction = false;
#endif
}

void UARPG_DA_RoleSelection_Interact::WhenActionActived()
{
	Super::WhenActionActived();
	if (ACharacterBase* TargetCharacter = Cast<ACharacterBase>(InteractTarget.Get()))
	{
		TargetCharacter->SetLockedTarget(Role.Get());
		TargetCharacter->StopMovement();
	}
}

void UARPG_DA_RoleSelection_Interact::WhenActionDeactived()
{
	Super::WhenActionDeactived();
	if (ACharacterBase * TargetCharacter = Cast<ACharacterBase>(InteractTarget.Get()))
	{
		TargetCharacter->SetLockedTarget(nullptr);
	}
}

bool UARPG_DA_RoleSelection_Interact::IsActionValid() const
{
	return Super::IsActionValid() && InteractTarget.IsValid() && InteractTarget->GetDistanceTo(Role.Get()) < ValidDistance;
}
