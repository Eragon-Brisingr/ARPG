// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CharacterType.h"
#include "ARPG_RelationshipFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_RelationshipFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "角色|关系", meta = (DisplayName = "Relationship >= Relationship", CompactNodeTitle = ">=", Keywords = ">= greater"))
	bool GreaterEqual_Relationship(ECharacterRelationship LHS, ECharacterRelationship RHS) { return LHS >= RHS; }

	UFUNCTION(BlueprintPure, Category = "角色|关系", meta = (DisplayName = "Relationship <= Relationship", CompactNodeTitle = "<=", Keywords = "<= less"))
	bool LessEqual_Relationship(ECharacterRelationship LHS, ECharacterRelationship RHS) { return LHS <= RHS; }

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<class ACharacterBase*> FilterByRelationshipTowards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, ECharacterRelationship Relationship);

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<class ACharacterBase*> FilterByRelationshipsTowards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, TArray<ECharacterRelationship> Relationships);

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<class ACharacterBase*> FilterByRelationshipGE_Towards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, ECharacterRelationship GE_Relationship);

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<class ACharacterBase*> FilterByRelationshipLE_Towards(class ACharacterBase* Character, const TArray<class ACharacterBase*>& FilterCharacters, ECharacterRelationship LE_Relationship);
};
