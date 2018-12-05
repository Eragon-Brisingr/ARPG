// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableActorManagerVisualizer.h"
#include "SceneManagement.h"
#include "ARPG_InteractableActorManager.h"

IMPLEMENT_HIT_PROXY(HInteractableActorManagerSingleVisProxy, HComponentVisProxy)

namespace PDIHelper
{
	void DrawArrow(FPrimitiveDrawInterface* PDI, const FVector& StartLocation, const FVector& EndLocation, const FLinearColor& Color)
	{
		FVector Vector = EndLocation - StartLocation;
		DrawDirectionalArrow(PDI, FTransform(Vector.Rotation(), StartLocation).ToMatrixNoScale(), Color, Vector.Size(), 5.f, SDPG_Foreground, 3.f);
	}
}

void FInteractableActorManagerSingleVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	InteractableActorManager_Simple = const_cast<UInteractableActorManagerSingle*>(Cast<UInteractableActorManagerSingle>(Component));
	if (InteractableActorManager_Simple.IsValid())
	{
		FTransform Transform = InteractableActorManager_Simple->GetOwner()->GetActorTransform();

		for (int32 i = 0; i < InteractableActorManager_Simple->Behaviors.Num(); ++i)
		{
			const FPositionWithBehavior& Behavior = InteractableActorManager_Simple->Behaviors[i];
			FVector WorldLocation = Transform.TransformPosition(Behavior.Location);
			FRotator WorldRotation = Transform.TransformRotation(Behavior.Rotation.Quaternion()).Rotator();
			FTransform WorldWidgetTransform(WorldRotation, WorldLocation);
			PDIHelper::DrawArrow(PDI, WorldLocation, WorldLocation + WorldRotation.Vector() * 100.f, FLinearColor::Red);

			const float WidgetSize = 0.035f;
			const float ZoomFactor = FMath::Min<float>(View->ViewMatrices.GetProjectionMatrix().M[0][0], View->ViewMatrices.GetProjectionMatrix().M[1][1]);
			const float WidgetRadius = View->Project(WorldWidgetTransform.GetTranslation()).W * (WidgetSize / ZoomFactor);
			PDI->SetHitProxy(new HInteractableActorManagerSingleVisProxy(Component, i));
			DrawWireDiamond(PDI, WorldWidgetTransform.ToMatrixNoScale(), WidgetRadius, bIsEditing ? FColor::White : FColor(128, 128, 255), SDPG_Foreground);
			PDI->SetHitProxy(NULL);
		}
	}
}

bool FInteractableActorManagerSingleVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if (VisProxy && VisProxy->Component.IsValid())
	{
		if (VisProxy->IsA(HInteractableActorManagerSingleVisProxy::StaticGetType()))
		{
			HInteractableActorManagerSingleVisProxy* InteractableActorManagerSimpleVisProxy = (HInteractableActorManagerSingleVisProxy*)VisProxy;
			EditIndex = InteractableActorManagerSimpleVisProxy->Index;
			bIsEditing = true;
		}
		return true;
	}
	return false;
}

bool FInteractableActorManagerSingleVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if (InteractableActorManager_Simple.IsValid() && EditIndex < InteractableActorManager_Simple->Behaviors.Num())
	{
		FTransform Transform = InteractableActorManager_Simple->GetOwner()->GetActorTransform();
		FVector WorldLocation = Transform.TransformPosition(InteractableActorManager_Simple->Behaviors[EditIndex].Location);
		OutLocation = WorldLocation;
		return true;
	}
	return false;
}

bool FInteractableActorManagerSingleVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale)
{
	if (InteractableActorManager_Simple.IsValid() && EditIndex < InteractableActorManager_Simple->Behaviors.Num())
	{
		InteractableActorManager_Simple->Behaviors[EditIndex].Location += DeltaTranslate;
		InteractableActorManager_Simple->Behaviors[EditIndex].Rotation += DeltalRotate;

		return true;
	}
	return false;
}

void FInteractableActorManagerSingleVisualizer::EndEditing()
{
	bIsEditing = false;
	EditIndex = 0;
	InteractableActorManager_Simple = nullptr;
}
