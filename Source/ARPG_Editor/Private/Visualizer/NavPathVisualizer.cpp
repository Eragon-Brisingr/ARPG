// Fill out your copyright notice in the Description page of Project Settings.

#include "NavPathVisualizer.h"
#include "ARPG_NavPath.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Behavior/ARPG_CharacterBehaviorBase.h"
#include "DrawDebugHelpers.h"
#include "MultiBoxBuilder.h"
#include "IDetailRootObjectCustomization.h"
#include "IDetailCustomization.h"
#include "ModuleManager.h"

#include "PropertyEditorModule.h"
#include "DetailLayoutBuilder.h"
#include "SScrollBox.h"
#include "DetailCategoryBuilder.h"

void DrawArrow(FPrimitiveDrawInterface* PDI, const FVector& StartLocation, const FVector& EndLocation, const FLinearColor& Color)
{
	FVector Vector = EndLocation - StartLocation;
	DrawDirectionalArrow(PDI, FTransform(Vector.Rotation(), StartLocation).ToMatrixNoScale(), Color, Vector.Size(), 5.f, SDPG_Foreground, 3.f);
}

void DrawNavPath(AARPG_NavPath* NavPath, int32 StartIndex, int32 EndIndex, FPrimitiveDrawInterface* PDI)
{
	const TArray<FARPG_NavPathPoint>& NavPathPoints = NavPath->NavPathPoints;
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(NavPath, NavPathPoints[StartIndex].Location, NavPathPoints[EndIndex].Location);
	if (Path && Path->IsValid())
	{
		for (int32 j = 1; j < Path->PathPoints.Num(); ++j)
		{
			DrawArrow(PDI, Path->PathPoints[j - 1], Path->PathPoints[j], FLinearColor::Blue);
		}
	}
	else
	{
		DrawArrow(PDI, NavPathPoints[StartIndex].Location, NavPathPoints[EndIndex].Location, FLinearColor::Red);
	}
}

int32 FNavPathVisualizer::SelectIndex;

void FNavPathVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FSplineComponentVisualizer::DrawVisualization(Component, View, PDI);

	if (const UARPG_NavPathVisualHelper* SplineComp = Cast<const UARPG_NavPathVisualHelper>(Component))
	{
		if (AARPG_NavPath* NavPath = Cast<AARPG_NavPath>(SplineComp->GetOwner()))
		{
			const TArray<FARPG_NavPathPoint>& NavPathPoints = NavPath->NavPathPoints;
			if (NavPathPoints.Num() < 2)
			{
				return;
			}

			for (int32 i = 1; i < NavPathPoints.Num(); ++i)
			{
				DrawNavPath(NavPath, i - 1, i, PDI);
			}
			if (NavPath->bIsClosedLoop)
			{
				DrawNavPath(NavPath, NavPathPoints.Num() - 1, 0, PDI);
			}

			for (const FARPG_NavPathPoint& Point : NavPathPoints)
			{
				DrawArrow(PDI, Point.Location, Point.Location + Point.Rotation.Vector() * 100.f, FColor::Yellow);
			}
		}
	}
}

bool FNavPathVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	bool Result = FSplineComponentVisualizer::VisProxyHandleClick(InViewportClient, VisProxy, Click);
	SelectIndex = LastKeyIndexSelected;
	return Result;
}

class SNavigationPathWidget : public SCompoundWidget
{
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

	class FNavPathCustomization : public IDetailCustomization
	{
	public:
		// IDetailCustomization interface
		virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
		{
			DetailBuilder.HideCategory(TEXT("Transform"));
			DetailBuilder.HideCategory(TEXT("Rendering"));
			DetailBuilder.HideCategory(TEXT("Replication"));
			DetailBuilder.HideCategory(TEXT("Input"));
			DetailBuilder.HideCategory(TEXT("Actor"));
			DetailBuilder.HideCategory(TEXT("LOD"));
			DetailBuilder.HideCategory(TEXT("Cooking"));
			DetailBuilder.HideCategory(TEXT("Tick"));

			TSharedRef<IPropertyHandle> Behaviors = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(AARPG_NavPath, NavPathPoints));
			TSharedPtr<IPropertyHandle> ActivedElement = Behaviors->GetChildHandle(FNavPathVisualizer::SelectIndex);
			IDetailCategoryBuilder& ActiveBehaviorRow = DetailBuilder.EditCategory(TEXT("选中的路点"));

			uint32 ChildNum;
			if (ActivedElement->GetNumChildren(ChildNum) == FPropertyAccess::Success)
			{
				for (uint32 i = 0; i < ChildNum; ++i)
				{
					ActiveBehaviorRow.AddProperty(ActivedElement->GetChildHandle(i));
				}
			}
		}
	};
public:
	SLATE_BEGIN_ARGS(SNavigationPathWidget)
		:_InObject(nullptr)
	{}
		SLATE_ARGUMENT(UObject*, InObject)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		FDetailsViewArgs DetailsViewArgs(true, true, true, FDetailsViewArgs::HideNameArea, true);
		DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
		DetailsViewArgs.bShowScrollBar = false;
		PropertyWidget = PropertyModule.CreateDetailView(DetailsViewArgs);
		PropertyWidget->SetRootObjectCustomizationInstance(MakeShareable(new FRootObjectCustomization));
		PropertyWidget->RegisterInstancedCustomPropertyLayout(AARPG_NavPath::StaticClass(), FOnGetDetailCustomizationInstance::CreateLambda([] {return MakeShareable(new FNavPathCustomization); }));
		ChildSlot
			[
 				SNew(SBox)
 				.WidthOverride(400.f)
 				.MaxDesiredHeight(400.f)
 				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					.HAlign(HAlign_Fill)
					[
						PropertyWidget.ToSharedRef()
					]
				]
			];

		PropertyWidget->SetObject(InArgs._InObject);
	}

	TSharedPtr<IDetailsView> PropertyWidget;
};

TSharedPtr<SWidget> FNavPathVisualizer::GenerateContextMenu() const
{
	TSharedPtr<SNavigationPathWidget> InteractableActorManagerWidget = SNew(SNavigationPathWidget).InObject(GetEditedSplineComponent()->GetOwner());
	return InteractableActorManagerWidget;
}
