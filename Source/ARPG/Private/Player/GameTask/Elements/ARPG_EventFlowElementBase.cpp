// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_EventFlowElementBase.h"
#include "CharacterBase.h"

ACharacterBase* UARPG_EventFlowElementBase::GetARPG_Character() const
{
	return Cast<ACharacterBase>(GetOwningCharacter());
}
