// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_ItemFactory.h"
#include "ARPG_ItemBase.h"
#include "KismetEditorUtilities.h"
#include "KismetCompilerModule.h"
#include "ModuleManager.h"
#include "ClassViewerModule.h"
#include "SClassPickerDialog.h"
#include "ARPG_Editor.h"
#include "ARPG_EditorSettings.h"

#define LOCTEXT_NAMESPACE "ARPG_ItemFactory"

class UARPG_EditorSettings;

UARPG_ItemFactory::UARPG_ItemFactory()
{
	bCreateNew = true;

	bEditAfterNew = true;

	SupportedClass = AARPG_ItemBase::StaticClass();
}

FText UARPG_ItemFactory::GetDisplayName() const
{
	return LOCTEXT("创造新道具", "创造新道具");
}

FText UARPG_ItemFactory::GetToolTip() const
{
	return LOCTEXT("创造新道具", "创造新道具");
}

FName UARPG_ItemFactory::GetNewAssetThumbnailOverride() const
{
	return TEXT("ClassThumbnail.BlueprintMacroLibrary");
}

uint32 UARPG_ItemFactory::GetMenuCategories() const
{
	return FARPG_EditorModule::ARPG_AssetCategory;
}

UObject* UARPG_ItemFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UClass* BlueprintClass = nullptr;
	UClass* BlueprintGeneratedClass = nullptr;

	IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	KismetCompilerModule.GetBlueprintTypesForClass(ItemClass, BlueprintClass, BlueprintGeneratedClass);

	return FKismetEditorUtilities::CreateBlueprint(ItemClass, InParent, InName, EBlueprintType::BPTYPE_Normal, BlueprintClass, BlueprintGeneratedClass);
}

bool UARPG_ItemFactory::ConfigureProperties()
{
	ItemClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;

	Options.Mode = EClassViewerMode::ClassPicker;
	Options.ClassFilter = MakeShareable<FARPG_ItemFilterViewer>(new FARPG_ItemFilterViewer);

	const FText TitleText = LOCTEXT("选择创造道具类型", "选择创造道具类型");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, AARPG_ItemBase::StaticClass());

	if (bPressedOk)
	{
		ItemClass = ChosenClass ? ChosenClass : AARPG_ItemBase::StaticClass();
	}

	return bPressedOk;
}

FARPG_ItemFilterViewer::FARPG_ItemFilterViewer()
{
	DisallowedClassFlags = CLASS_Deprecated;
	for (UClass* AllowedCreateParentClass : GetDefault<UARPG_EditorSettings>()->AllowedCreateParentClasses)
	{
		AllowedChildrenOfClasses.Add(AllowedCreateParentClass);
	}
}

bool FARPG_ItemFilterViewer::IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	return !InClass->HasAnyClassFlags(DisallowedClassFlags) && InFilterFuncs->IfInClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
}

bool FARPG_ItemFilterViewer::IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags) && InFilterFuncs->IfInClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
}

#undef LOCTEXT_NAMESPACE
