// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AlertSystemBase.h"
#include "CharacterBase.h"

class UWorld* UARPG_AlertSystemBase::GetWorld() const
{
	return Character->GetWorld();
}

float UARPG_AlertSystemBase::GetSightAddAlertValueFilterRelationship(const class ACharacterBase* LookedCharacter) const
{
	if (Character->GetRelationshipTowards(LookedCharacter) <= Character->MaxAlertRelationship)
	{
		return GetSightAddAlertValue(LookedCharacter);
	}
	return 0.f;
}

float UARPG_AlertSystemBase::GetHearAddAlertValueFilterRelationship(const class ACharacterBase* HeardCharacter, const FVector& SimulusLocation, float Strength) const
{
	if (Character->GetRelationshipTowards(HeardCharacter) <= Character->MaxAlertRelationship)
	{
		return GetHearAddAlertValue(HeardCharacter, SimulusLocation, Strength);
	}
	return 0.f;
}

float UARPG_AlertSystemBase::GetNoseAddAlertValueFilterRelationship(const class ACharacterBase* NosedCharacter) const
{
	if (Character->GetRelationshipTowards(NosedCharacter) <= Character->MaxAlertRelationship)
	{
		return GetNoseAddAlertValue(NosedCharacter);
	}
	return 0.f;
}
