// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplineComponentVisualizer.h"

/**
 * 
 */
class ARPG_EDITOR_API FNavPathVisualizer : public FSplineComponentVisualizer
{
public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
};
