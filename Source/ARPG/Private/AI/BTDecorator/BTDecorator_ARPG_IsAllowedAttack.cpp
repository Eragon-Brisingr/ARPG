// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_ARPG_IsAllowedAttack.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_ARPG_IsAllowedAttack::UBTDecorator_ARPG_IsAllowedAttack()
{
	NodeName = TEXT("是否允许攻击");
}

bool UBTDecorator_ARPG_IsAllowedAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(AIOwner->GetPawn()))
	{
		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
			if (AActor* TargetActor = Cast<AActor>(KeyValue))
			{
				return IARPG_AI_BattleInterface::IsAllowedAttack(Character->BattleControl.GetObject(), TargetActor);
			}
		}
	}
	return false;
}
