// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ARPG_AttackTarget.h"
#include "AIController.h"
#include "CharacterBase.h"

UBTTask_ARPG_AttackTarget::UBTTask_ARPG_AttackTarget()
{
	NodeName = TEXT("攻击目标");
}

EBTNodeResult::Type UBTTask_ARPG_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		//Character->BattleStyleSystem->BattleControl->
	}
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ARPG_AttackTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UBTTask_ARPG_AttackTarget::WhenAttackFinished()
{

}
