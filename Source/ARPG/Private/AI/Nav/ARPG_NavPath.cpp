// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_NavPath.h"
#include "ARPG_AD_CharacterBase.h"
#include "CharacterBase.h"
#include "ARPG_AI_Config.h"

int32 UARPG_FollowPathMoveConfigBase::GetStartFollowPathMoveIndex(ACharacterBase* Mover, class AARPG_NavPath* Path, int32 RememberIndex) const
{
	return RememberIndex != INDEX_NONE ? RememberIndex : 0;
}

float FARPG_NavPathPoint::GetAcceptableRadius() const
{
	return bAttachToLocation ? 0.f : AcceptableRadius;
}

UARPG_AD_CharacterBase* FARPG_NavPathPoint::GetBehavior(ACharacterBase* Invoker)
{
	UARPG_AD_CharacterBase*& Behavior = BehaviorMap.FindOrAdd(Invoker);
	if (Behavior == nullptr)
	{
		Behavior = ::DuplicateObject(BehaviorTemplate, Invoker);
		Behavior->Character = Invoker;
		Behavior->InitLeader(Invoker);
	}
	return Behavior;
}

// Sets default values
AARPG_NavPath::AARPG_NavPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	VisualControl = CreateEditorOnlyDefaultSubobject<UARPG_NavPathVisualHelper>(GET_MEMBER_NAME_CHECKED(AARPG_NavPath, VisualControl));
	if (VisualControl)
	{
		VisualControl->bIsEditorOnly = true;
	}

	SetRootComponent(VisualControl);
#endif
}

#if WITH_EDITOR
void AARPG_NavPath::OnConstruction(const FTransform& Transform)
{
	int32 PreNum = NavPathPoints.Num();

	NavPathPoints.SetNum(VisualControl->GetNumberOfSplinePoints());
	for (int32 i = 0; i < NavPathPoints.Num(); ++i)
	{
		NavPathPoints[i].Location = VisualControl->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		NavPathPoints[i].Rotation = VisualControl->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World);
	}
	TSoftClassPtr<UARPG_AD_CharacterBase> PathFollowDefaultActionClass = GetDefault<UARPG_AI_Config>()->PathFollowDefaultActionClass;
	if (!PathFollowDefaultActionClass.IsNull())
	{
		for (int32 i = PreNum; i < NavPathPoints.Num(); ++i)
		{
			if (NavPathPoints[i].BehaviorTemplate == nullptr)
			{
				NavPathPoints[i].BehaviorTemplate = NewObject<UARPG_AD_CharacterBase>(this, PathFollowDefaultActionClass.LoadSynchronous());
				NavPathPoints[i].BehaviorTemplate->SetFlags(EObjectFlags::RF_ArchetypeObject);
			}
		}
	}

	VisualControl->SetClosedLoop(bIsClosedLoop);
}

void AARPG_NavPath::DrawGizmoNative(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI, bool IsSelected)
{
	for (FARPG_NavPathPoint& NavPathPoint : NavPathPoints)
	{
		if (NavPathPoint.bAttachToRotation)
		{
			DrawDirectionalArrow(PDI, FTransform(NavPathPoint.Rotation, NavPathPoint.Location).ToMatrixWithScale(), FColor::Red, 50.f, 5.f, SDPG_World, 1.f);
		}
	}
}

#endif
