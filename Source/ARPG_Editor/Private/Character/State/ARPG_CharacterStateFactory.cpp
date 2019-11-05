// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterStateFactory.h"
#include "ModuleManager.h"
#include "ClassViewerModule.h"
#include "SClassPickerDialog.h"
#include "ClassViewerFilter.h"
#include "KismetEditorUtilities.h"

#include "ARPG_CharacterStateBase.h"
#include "ARPG_CharacterStateBlueprint.h"
#include "ARPG_CharacterStateClass.h"

#define LOCTEXT_NAMESPACE "ARPG_CharacterState"

UARPG_CharacterStateFactory::UARPG_CharacterStateFactory()
{
	bCreateNew = true;

	bEditAfterNew = true;

	SupportedClass = UARPG_CharacterStateBase::StaticClass();
}

FText UARPG_CharacterStateFactory::GetDisplayName() const
{
	return LOCTEXT("创造新角色状态（Buff）", "创造新角色状态（Buff）");
}

FText UARPG_CharacterStateFactory::GetToolTip() const
{
	return LOCTEXT("创造新角色状态（Buff）", "创造新角色状态（Buff）");
}

UObject* UARPG_CharacterStateFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FKismetEditorUtilities::CreateBlueprint(CharacterStateClass, InParent, InName, EBlueprintType::BPTYPE_Normal, UARPG_CharacterStateBlueprint::StaticClass(), UARPG_CharacterStateClass::StaticClass());
}

bool UARPG_CharacterStateFactory::ConfigureProperties()
{
	CharacterStateClass = nullptr;

	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	FClassViewerInitializationOptions Options;

	Options.Mode = EClassViewerMode::ClassPicker;

	class FXD_ItemCoreFilterViewer : public IClassViewerFilter
	{
	public:
		const EClassFlags DisallowedClassFlags = CLASS_Deprecated;
		const EClassFlags AllowedClassFlags = CLASS_AllFlags;

		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
		{
			return InClass->HasAnyClassFlags(AllowedClassFlags) && !InClass->HasAnyClassFlags(DisallowedClassFlags) && InClass->IsChildOf<UARPG_CharacterStateBase>();
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
		{
			return InUnloadedClassData->HasAnyClassFlags(AllowedClassFlags) && !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags) && InUnloadedClassData->IsChildOf(UARPG_CharacterStateBase::StaticClass());
		}
	};

	Options.ClassFilter = MakeShareable<FXD_ItemCoreFilterViewer>(new FXD_ItemCoreFilterViewer);
	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::Dynamic;
	Options.DisplayMode = EClassViewerDisplayMode::TreeView;

	const FText TitleText = LOCTEXT("创造新角色状态（Buff）", "创造新角色状态（Buff）");
	UClass* ChosenClass = nullptr;
	const bool bPressedOk = SClassPickerDialog::PickClass(TitleText, Options, ChosenClass, UARPG_CharacterStateBase::StaticClass());

	if (bPressedOk)
	{
		CharacterStateClass = ChosenClass;
	}

	return bPressedOk;
}

#undef LOCTEXT_NAMESPACE
