// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualizerRegister.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "ARPG_NavPath.h"
#include "NavPathVisualizer.h"

template<typename UComponent, typename FVisualizer>
void RegisterComponentVisualizer()
{
	TSharedPtr<FVisualizer> Visualizer = MakeShareable(new FVisualizer);
	GUnrealEd->RegisterComponentVisualizer(UComponent::StaticClass()->GetFName(), Visualizer);
	Visualizer->OnRegister();
}

template<typename UComponent>
void UnregisterComponentVisualizer()
{
	GUnrealEd->UnregisterComponentVisualizer(UComponent::StaticClass()->GetFName());
}

void FVisualizerRegister::RegisterVisualizer()
{
	if (GUnrealEd)
	{
		RegisterComponentVisualizer<UARPG_NavPathVisualHelper, FNavPathVisualizer>();
	}
}

void FVisualizerRegister::UnregisterVisualizer()
{
	if (GUnrealEd)
	{
		UnregisterComponentVisualizer<UARPG_NavPathVisualHelper>();
	}
}
