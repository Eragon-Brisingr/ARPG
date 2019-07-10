// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_SaveGameSystemBase.h"
#include "ARPG_SaveGameSystem.generated.h"

/**
 * 
 */
namespace ARPG_SaveGameVersion
{
	enum Type
	{
		Original,
	};
}

UCLASS()
class ARPG_API UARPG_SaveGameSystem : public UXD_SaveGameSystemBase
{
	GENERATED_BODY()
public:
	UARPG_SaveGameSystem();
};
