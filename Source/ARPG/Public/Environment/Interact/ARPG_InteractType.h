// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ARPG_InteractType.generated.h"

/**
 * 
 */
UENUM()
enum class EInteractResult
{
	CanNotInteract,
	InteractedFailed,
	InteractedSucceed
};

DECLARE_DELEGATE_OneParam(FOnInteractFinished, EInteractResult /*Result*/);

DECLARE_DELEGATE(FOnInteractAbortFinished);
