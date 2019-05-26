// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CharacterType.h"
#include "ARPG_RelationshipFunctionLibrary.generated.h"

class ACharacterBase;

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
	static TArray<ACharacterBase*> FilterByRelationshipTowards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, ECharacterRelationship Relationship);

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<ACharacterBase*> FilterByRelationshipsTowards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, TArray<ECharacterRelationship> Relationships);

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<ACharacterBase*> FilterByRelationshipGE_Towards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, ECharacterRelationship GE_Relationship);

	UFUNCTION(BlueprintCallable, Category = "角色|关系", meta = (DeterminesOutputType = "FilterCharacters"))
	static TArray<ACharacterBase*> FilterByRelationshipLE_Towards(const ACharacterBase* Character, const TArray<ACharacterBase*>& FilterCharacters, ECharacterRelationship LE_Relationship);
};
