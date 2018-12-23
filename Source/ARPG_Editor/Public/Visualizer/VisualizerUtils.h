// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

class IDetailLayoutBuilder;
class IDetailsView;

/**
 * 
 */
namespace FVisualizerUtils
{
	void HideActorComponentCategory(IDetailLayoutBuilder &DetailBuilder);

	class SRightMenuEditorObjectWidget : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SRightMenuEditorObjectWidget)
		{}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);

		TSharedPtr<IDetailsView> PropertyWidget;
	};

};
