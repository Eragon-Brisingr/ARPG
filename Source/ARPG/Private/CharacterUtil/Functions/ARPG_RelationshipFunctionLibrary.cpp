// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_RelationshipFunctionLibrary.h"
#include "CharacterBase.h"

template<typename TPredicate>
static TArray<class ACharacterBase*> FilterCharacterByPredicate(const TArray<class ACharacterBase*>& FilterCharacters, const TPredicate& Predicate)
{
	TArray<class ACharacterBase*> Res;
	for (class ACharacterBase* Element : FilterCharacters)
	{
		if (Element && Predicate(Element))
		{
			Res.Add(Element);
		}
	}
	return Res;
}

TArray<class ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipTowards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, ECharacterRelationship Relationship)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Character->GetRelationshipTowards(Element) == Relationship; }) : TArray<class ACharacterBase*>();
}

TArray<class ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipsTowards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, TArray<ECharacterRelationship> Relationships)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Relationships.Contains(Character->GetRelationshipTowards(Element)); }) : TArray<class ACharacterBase*>();
}

TArray<class ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipGE_Towards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, ECharacterRelationship GE_Relationship)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Character->GetRelationshipTowards(Element) >= GE_Relationship; }) : TArray<class ACharacterBase*>();
}

TArray<class ACharacterBase*> UARPG_RelationshipFunctionLibrary::FilterByRelationshipLE_Towards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, ECharacterRelationship LE_Relationship)
{
	return Character ? FilterCharacterByPredicate(FilterCharacters, [&](ACharacterBase* Element) {return Character->GetRelationshipTowards(Element) <= LE_Relationship; }) : TArray<class ACharacterBase*>();
}
