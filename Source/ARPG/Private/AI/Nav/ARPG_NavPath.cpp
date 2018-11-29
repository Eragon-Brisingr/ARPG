// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_NavPath.h"

// Sets default values
AARPG_NavPath::AARPG_NavPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	VisualControl = CreateEditorOnlyDefaultSubobject<UARPG_NavPathVisualHelper>(GET_MEMBER_NAME_CHECKED(AARPG_NavPath, VisualControl));
	SetRootComponent(VisualControl);
#endif
}

#if WITH_EDITORONLY_DATA
void AARPG_NavPath::OnConstruction(const FTransform& Transform)
{
	NavPathPoints.SetNum(VisualControl->GetNumberOfSplinePoints());
	for (int32 i = 0; i < NavPathPoints.Num(); ++i)
	{
		NavPathPoints[i].Location = VisualControl->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
	}
}
#endif
