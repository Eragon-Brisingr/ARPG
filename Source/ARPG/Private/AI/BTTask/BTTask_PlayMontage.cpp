// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayMontage.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

EBTNodeResult::Type UBTTask_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (MontageToPlay)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			Character->PlayMontageWithBlendingOutDelegate(MontageToPlay, FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=, P_OwnerComp = &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
				{
					FinishLatentTask(*P_OwnerComp, bInterrupted == false ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
				}));
			return EBTNodeResult::InProgress;
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_PlayMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (MontageToPlay)
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ACharacterBase* Character = Cast<ACharacterBase>(MyController->GetPawn()))
		{
			if (bStopMontageWhenAborted)
			{
				Character->StopMontage(MontageToPlay);
				return EBTNodeResult::Aborted;
			}
			else
			{
				FOnMontageBlendingOutStarted OnMontageBlendingOutStarted = FOnMontageBlendingOutStarted::CreateWeakLambda(this, [=, P_OwnerComp = &OwnerComp](UAnimMontage* Montage, bool bInterrupted)
					{
						FinishLatentAbort(*P_OwnerComp);
					});
				Character->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(const_cast<FOnMontageBlendingOutStarted&>(OnMontageBlendingOutStarted), MontageToPlay);
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Aborted;
}
