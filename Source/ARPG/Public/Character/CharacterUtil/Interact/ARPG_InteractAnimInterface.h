// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPG_InteractAnimInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARPG_InteractAnimInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IARPG_InteractAnimInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "角色|交互")
	void SetEnableInteractHandIK(bool Enable);
	static void SetEnableInteractHandIK(UObject* Obj, bool Enable) { IARPG_InteractAnimInterface::Execute_SetEnableInteractHandIK(Obj, Enable); }

	UFUNCTION(BlueprintImplementableEvent, Category = "角色|交互")
	void SetInteractHandIK_Location(const FVector& Location);
	static void SetInteractHandIK_Location(UObject* Obj, const FVector& Location) { IARPG_InteractAnimInterface::Execute_SetInteractHandIK_Location(Obj, Location); }
};
