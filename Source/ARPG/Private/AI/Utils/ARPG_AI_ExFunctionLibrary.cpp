// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AI_ExFunctionLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"

void UARPG_AI_ExFunctionLibrary::SetDynamicSubtree(class AAIController* AIController, FGameplayTag InjectTag, class UBehaviorTree* BehaviorAsset)
{
	if (AIController)
	{
		if (UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent()))
		{
			BehaviorTreeComponent->SetDynamicSubtree(InjectTag, BehaviorAsset);
		}
	}
}
