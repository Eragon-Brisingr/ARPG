// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ARPG_AttackTarget.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ARPG_AttackTarget::UBTTask_ARPG_AttackTarget()
{
	bNotifyTick = true;

	NodeName = TEXT("攻击目标");
}

EBTNodeResult::Type UBTTask_ARPG_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		if (Character->BattleControl)
		{
			if (AActor* Target = Cast<AActor>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID())))
			{
				IARPG_AI_BattleInterface::InvokeAttack(Character->BattleControl.GetObject(), Target, FARPG_OnAttackFinished::CreateUObject(this, &UBTTask_ARPG_AttackTarget::WhenAttackFinished, &OwnerComp, NodeMemory));
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_ARPG_AttackTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		if (Character->BattleControl)
		{
			if (AActor* Target = Cast<AActor>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID())))
			{
				IARPG_AI_BattleInterface::AbortAttack(Character->BattleControl.GetObject(), Target, FARPG_OnAttackAborted::CreateUObject(this, &UBTTask_ARPG_AttackTarget::WhenAttackAborted, &OwnerComp, NodeMemory));
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Aborted;
}

void UBTTask_ARPG_AttackTarget::WhenAttackFinished(bool Succeed, UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	FinishLatentTask(*OwnerComp, Succeed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
}

void UBTTask_ARPG_AttackTarget::WhenAttackAborted(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory)
{
	FinishLatentAbort(*OwnerComp);
}

void UBTTask_ARPG_AttackTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
	{
		if (Character->BattleControl)
		{
			if (AActor* Target = Cast<AActor>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID())))
			{
				IARPG_AI_BattleInterface::AttackingTick(Character->BattleControl.GetObject(), Target, DeltaSeconds);
			}
		}
	}
}
