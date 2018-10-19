// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_Editor.h"
#include <PropertyEditorModule.h>
#include "XD_PropertyCustomizationEx.h"
#include "ARPG_Item_Customization.h"

#define LOCTEXT_NAMESPACE "ARPG_Editor"

void FARPG_EditorModule::StartupModule()
{
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		RegisterCustomProperty(struct FARPG_Item, FARPG_Item_Customization);
	}
}


void FARPG_EditorModule::ShutdownModule()
{

}

IMPLEMENT_MODULE(FARPG_EditorModule, ARPG_Editor);

#undef LOCTEXT_NAMESPACE