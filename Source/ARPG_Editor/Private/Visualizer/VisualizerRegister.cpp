// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualizerRegister.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "ARPG_NavPath.h"
#include "NavPathVisualizer.h"
#include "ARPG_InteractableActorManager.h"
#include "InteractableActorManagerVisualizer.h"
#include "ARPG_AttackAreaManager.h"
#include "AttackAreaManagerVisualizer.h"

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
		RegisterComponentVisualizer<UInteractableActorManagerSingle, FInteractableActorManagerSingleVisualizer>();
		RegisterComponentVisualizer<UARPG_AttackAreaManager, FAttackAreaManagerVisualizer>();
	}
}

void FVisualizerRegister::UnregisterVisualizer()
{
	if (GUnrealEd)
	{
		UnregisterComponentVisualizer<UARPG_NavPathVisualHelper>();
		UnregisterComponentVisualizer<UInteractableActorManagerSingle>();
		UnregisterComponentVisualizer<UARPG_AttackAreaManager>();
	}
}
