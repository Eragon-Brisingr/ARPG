// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AI_ExFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

class UAISenseConfig* UARPG_AI_ExFunctionLibrary::GetSenseConfig(class UAIPerceptionComponent* AIPerceptionComponent, TSubclassOf<class UAISense> AISenseType)
{
	if (AIPerceptionComponent)
	{
		FAISenseID Id = UAISense::GetSenseID(AISenseType);
		if (Id.IsValid())
		{
			return AIPerceptionComponent->GetSenseConfig(Id);
		}
	}
	return nullptr;
}
