// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ClassViewerFilter.h"
#include "ARPG_ItemFactory.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_EDITOR_API UARPG_ItemFactory : public UFactory
{
	GENERATED_BODY()
public:
	UARPG_ItemFactory();

	UPROPERTY(EditAnywhere, Category = "道具")
	TSubclassOf<class AARPG_ItemBase> ItemClass;

	virtual FText GetDisplayName() const override;
	virtual FText GetToolTip() const override;
	virtual FName GetNewAssetThumbnailOverride() const override;
	virtual uint32 GetMenuCategories() const override;

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ConfigureProperties() override;
};

class FARPG_ItemFilterViewer : public IClassViewerFilter
{
public:
	FARPG_ItemFilterViewer();

	TSet<const UClass*> AllowedChildrenOfClasses;
	EClassFlags DisallowedClassFlags;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override;

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const class IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs) override;
};
