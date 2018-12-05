// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableActorManagerVisualizer.h"
#include "SceneManagement.h"
#include "ARPG_InteractableActorManager.h"

IMPLEMENT_HIT_PROXY(HInteractableActorManagerSimpleVisProxy, HComponentVisProxy)

namespace PDIHelper
{
	void DrawArrow(FPrimitiveDrawInterface* PDI, const FVector& StartLocation, const FVector& EndLocation, const FLinearColor& Color)
	{
		FVector Vector = EndLocation - StartLocation;
		DrawDirectionalArrow(PDI, FTransform(Vector.Rotation(), StartLocation).ToMatrixNoScale(), Color, Vector.Size(), 5.f, SDPG_Foreground, 3.f);
	}
}

void FInteractableActorManagerSimpleVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	InteractableActorManager_Simple = const_cast<UInteractableActorManager_Simple*>(Cast<UInteractableActorManager_Simple>(Component));
	if (InteractableActorManager_Simple.IsValid())
	{
		FTransform Transform = InteractableActorManager_Simple->GetOwner()->GetActorTransform();
		FVector WorldLocation = Transform.TransformPosition(InteractableActorManager_Simple->Location);
		FRotator WorldRotation = Transform.TransformRotation(InteractableActorManager_Simple->Rotation.Quaternion()).Rotator();
		FTransform WorldWidgetTransform(WorldRotation, WorldLocation);
		PDIHelper::DrawArrow(PDI, WorldLocation, WorldLocation + WorldRotation.Vector() * 100.f, FLinearColor::Red);

		const float WidgetSize = 0.035f;
		const float ZoomFactor = FMath::Min<float>(View->ViewMatrices.GetProjectionMatrix().M[0][0], View->ViewMatrices.GetProjectionMatrix().M[1][1]);
		const float WidgetRadius = View->Project(WorldWidgetTransform.GetTranslation()).W * (WidgetSize / ZoomFactor);
		PDI->SetHitProxy(new HInteractableActorManagerSimpleVisProxy(Component));
		DrawWireDiamond(PDI, WorldWidgetTransform.ToMatrixNoScale(), WidgetRadius, bIsEditing ? FColor::White : FColor(128, 128, 255), SDPG_Foreground);
		PDI->SetHitProxy(NULL);
	}
}

bool FInteractableActorManagerSimpleVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if (VisProxy && VisProxy->Component.IsValid())
	{
		bIsEditing = true;
		return true;
	}
	return false;
}

bool FInteractableActorManagerSimpleVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if (InteractableActorManager_Simple.IsValid())
	{
		FTransform Transform = InteractableActorManager_Simple->GetOwner()->GetActorTransform();
		FVector WorldLocation = Transform.TransformPosition(InteractableActorManager_Simple->Location);
		OutLocation = WorldLocation;
		return true;
	}
	return false;
}

bool FInteractableActorManagerSimpleVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale)
{
	if (InteractableActorManager_Simple.IsValid())
	{
		InteractableActorManager_Simple->Location += DeltaTranslate;
		InteractableActorManager_Simple->Rotation += DeltalRotate;

		return true;
	}
	return false;
}

void FInteractableActorManagerSimpleVisualizer::EndEditing()
{
	bIsEditing = false;
	InteractableActorManager_Simple = nullptr;
}
