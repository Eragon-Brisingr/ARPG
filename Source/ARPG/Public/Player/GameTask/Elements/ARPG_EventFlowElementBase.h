// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/XD_EventFlowElementBase.h"
#include "ARPG_EventFlowElementBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS(abstract)
class ARPG_API UARPG_EventFlowElementBase : public UXD_EventFlowElementBase
{
	GENERATED_BODY()
public:
	ACharacterBase* GetARPG_Character() const;
};
