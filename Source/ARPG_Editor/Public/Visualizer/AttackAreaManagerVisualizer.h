// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"
//#include "AttackAreaManagerVisualizer.generated.h"

class UARPG_AttackAreaManager;

/**
 * 
 */
struct FAttackAreaVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	FAttackAreaVisProxy(const UActorComponent* InComponent, int32 Index)
		: HComponentVisProxy(InComponent, HPP_Wireframe), Index(Index)
	{}

	int32 Index;
};

class ARPG_EDITOR_API FAttackAreaManagerVisualizer : public FComponentVisualizer
{
public:
	void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
	bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale) override;
	void EndEditing() override;
	virtual bool IsVisualizingArchetype() const;

	TSharedPtr<SWidget> GenerateContextMenu() const override;

	static int32 EditIndex;
	TWeakObjectPtr<UARPG_AttackAreaManager> AttackAreaManagerW;
};

