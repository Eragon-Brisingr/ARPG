// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ARPG_InteractType.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnInteractFinished, bool /*Succeed*/);

DECLARE_DELEGATE(FOnInteractAbortFinished);
