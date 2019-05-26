// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_RelationshipFunctionLibrary.h"
#include "CharacterBase.h"

template<typename TPredicate>
static TArray<ACharacterBase*> FilterCharacterByPredicate(const TArray<ACharacterBase*>& FilterCharacters, const TPredicate& Predicate)
{
	TArray<ACharacterBase*> Res;
	for (ACharacterBase* Element : FilterCharacters)
	{
		if (Element && Predicate(Element))
		{
			Res.Add(Element);
		}
	}
	return Res;
}

TArray<ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipTowards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, ECharacterRelationship Relationship)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Character->GetRelationshipTowards(Element) == Relationship; }) : TArray<ACharacterBase*>();
}

TArray<ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipsTowards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, TArray<ECharacterRelationship> Relationships)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Relationships.Contains(Character->GetRelationshipTowards(Element)); }) : TArray<ACharacterBase*>();
}

TArray<ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipGE_Towards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, ECharacterRelationship GE_Relationship)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Character->GetRelationshipTowards(Element) >= GE_Relationship; }) : TArray<ACharacterBase*>();
}

TArray<ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipLE_Towards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, ECharacterRelationship LE_Relationship)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Character->GetRelationshipTowards(Element) <= LE_Relationship; }) : TArray<ACharacterBase*>();
}
