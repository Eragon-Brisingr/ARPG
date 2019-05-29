// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_ARPG_CharacterCondition.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_CharacterConditionBase.h"

UBTD_ARPG_CharacterCondition::UBTD_ARPG_CharacterCondition()
{
	bNotifyTick = true;
}

bool UBTD_ARPG_CharacterCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (ACharacterBase* Character = Cast<ACharacterBase>(AIOwner->GetPawn()))
	{
		if (Condition)
		{
			return Condition.GetDefaultObject()->CalculateConditionValue(Character);
		}
		return true;
	}
	return false;
}

FString UBTD_ARPG_CharacterCondition::GetStaticDescription() const
{
	return FString::Printf(TEXT("角色条件%s"), *UXD_DebugFunctionLibrary::GetDebugName(Condition));
}

void UBTD_ARPG_CharacterCondition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
	{
		OwnerComp.RequestExecution(this);
	}
}
