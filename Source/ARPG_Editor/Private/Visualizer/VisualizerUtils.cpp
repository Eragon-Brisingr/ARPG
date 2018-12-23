// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualizerUtils.h"
#include "DetailLayoutBuilder.h"
#include "ModuleManager.h"
#include "SBox.h"
#include "SScrollBox.h"
#include "IDetailRootObjectCustomization.h"

TArray<FName> HideActorComponentCategoryName = { TEXT("Tags"), TEXT("ComponentReplication"), TEXT("Activation"),TEXT("Variable"),TEXT("Cooking"),TEXT("AssetUserData"),TEXT("Collision") };

void FVisualizerUtils::HideActorComponentCategory(IDetailLayoutBuilder &DetailBuilder)
{
	for (const FName& Name : HideActorComponentCategoryName)
	{
		DetailBuilder.HideCategory(Name);
	}
}

class FRootObjectCustomization : public IDetailRootObjectCustomization
{
	TSharedPtr<SWidget> CustomizeObjectHeader(const UObject* InRootObject) override
	{
		return SNullWidget::NullWidget;
	}
	bool IsObjectVisible(const UObject* InRootObject) const override
	{
		return true;
	}
	bool ShouldDisplayHeader(const UObject* InRootObject) const override
	{
		return false;
	}
};

void FVisualizerUtils::SRightMenuEditorObjectWidget::Construct(const FArguments& InArgs)
{
	auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	PropertyWidget = PropertyModule.CreateDetailView(DetailsViewArgs);
	PropertyWidget->SetRootObjectCustomizationInstance(MakeShareable(new FRootObjectCustomization));
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(400.f)
		.MaxDesiredHeight(400.f)
		[
			SNew(SScrollBox)
			+SScrollBox::Slot()
			.HAlign(HAlign_Fill)
			[
				PropertyWidget.ToSharedRef()
			]
		]
	];
}
