// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class UInteractableActorManagerSingle;

/**
 * 
 */
struct HInteractableActorManagerSingleVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HInteractableActorManagerSingleVisProxy(const UActorComponent* InComponent, int32 Index)
		: HComponentVisProxy(InComponent, HPP_Wireframe), Index(Index)
	{}

	int32 Index;
};

class ARPG_EDITOR_API FInteractableActorManagerSingleVisualizer : public FComponentVisualizer
{
public:
	void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale) override;
	void EndEditing() override;
	TSharedPtr<SWidget> GenerateContextMenu() const override;

	static int32 EditIndex;
	uint8 bIsEditing : 1;

	TWeakObjectPtr<UInteractableActorManagerSingle> InteractableActorManager_Simple;
};
