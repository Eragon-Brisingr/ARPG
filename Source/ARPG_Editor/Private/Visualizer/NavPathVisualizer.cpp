// Fill out your copyright notice in the Description page of Project Settings.

#include "NavPathVisualizer.h"
#include "ARPG_NavPath.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"


void DrawPath(FPrimitiveDrawInterface* PDI, FVector StartLocation, FVector EndLocation, const FLinearColor& Color)
{
	FVector Vector = EndLocation - StartLocation;
	DrawDirectionalArrow(PDI, FTransform(Vector.Rotation(), StartLocation).ToMatrixNoScale(), Color, Vector.Size(), 5.f, SDPG_Foreground, 3.f);
}

void FNavPathVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FSplineComponentVisualizer::DrawVisualization(Component, View, PDI);

	if (const UARPG_NavPathVisualHelper* SplineComp = Cast<const UARPG_NavPathVisualHelper>(Component))
	{
		if (const AARPG_NavPath* NavPath = Cast<AARPG_NavPath>(SplineComp->GetOwner()))
		{
			const TArray<FARPG_NavPathPoint>& NavPathPoints = NavPath->NavPathPoints;
			for (int32 i = 1; i < NavPathPoints.Num(); ++i)
			{
				UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(const_cast<UActorComponent*>(Component), NavPathPoints[i].Location, NavPathPoints[i - 1].Location);
				if (Path && Path->IsValid())
				{
					for (int32 j = 1; j < Path->PathPoints.Num(); ++j)
					{
						DrawPath(PDI, Path->PathPoints[j - 1], Path->PathPoints[j], FLinearColor::Blue);
					}
				}
				else
				{
					DrawPath(PDI, NavPathPoints[i - 1].Location, NavPathPoints[i].Location, FLinearColor::Red);
				}
			}
		}
	}
}
