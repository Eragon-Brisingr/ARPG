// Fill out your copyright notice in the Description page of Project Settings.

#include "NavPathVisualizer.h"
#include "ARPG_NavPath.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Behavior/ARPG_CharacterBehaviorBase.h"
#include "DrawDebugHelpers.h"
#include "MultiBoxBuilder.h"


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

TSharedPtr<SWidget> FNavPathVisualizer::GenerateContextMenu() const
{
	return nullptr;
}
