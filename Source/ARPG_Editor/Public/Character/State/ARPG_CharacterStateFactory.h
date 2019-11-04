// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ARPG_CharacterStateFactory.generated.h"

class UARPG_CharacterStateBase;

/**
 * 
 */
UCLASS()
class ARPG_EDITOR_API UARPG_CharacterStateFactory : public UFactory
{
	GENERATED_BODY()
public:
	UARPG_CharacterStateFactory();

	UPROPERTY(EditAnywhere, Category = "State")
	TSubclassOf<UARPG_CharacterStateBase> CharacterStateClass;

	FText GetDisplayName() const override;
	FText GetToolTip() const override;
	uint32 GetMenuCategories() const override;

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	bool ConfigureProperties() override;
};
