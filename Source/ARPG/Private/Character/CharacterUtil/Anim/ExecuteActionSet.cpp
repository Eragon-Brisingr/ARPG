// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecuteActionSet.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "CharacterBase.h"
#include "ARPG_CollisionType.h"



bool FExecuteActionSet::InvokeExecuteOther(class ACharacterBase* Invoker, class ACharacterBase* ExecuteTarget) const
{
	//考虑特殊处决
	if (UAnimMontage* ExecuteTargetCurrentMontage = ExecuteTarget->GetCurrentMontage())
	{
		if (const FExecuteAnimData* ExecuteAnimData = ExecuteMontageMap.Find(ExecuteTargetCurrentMontage))
		{
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ExecuteTarget->GetActorLocation(), Invoker->GetActorLocation());
			FRotator CompareRotation = UKismetMathLibrary::FindLookAtRotation(ExecuteTarget->GetActorLocation(), ExecuteTarget->GetActorLocation() + ExecuteTarget->GetActorRotation().RotateVector(ExecuteAnimData->RelationLocation));
			if (FMath::Abs(FRotator::NormalizeAxis(LookAtRotation.Yaw - CompareRotation.Yaw)) <= ExecuteAnimData->Tolerance)
			{
				FVector TargetLocation(Invoker->GetActorLocation() - Invoker->GetActorRotation().RotateVector(ExecuteAnimData->RelationLocation));
				TargetLocation.Z = Invoker->GetActorLocation().Z;
				FRotator TargetRotation(Invoker->GetActorRotation().Pitch, LookAtRotation.Yaw + ExecuteAnimData->YawOffset, Invoker->GetActorRotation().Roll);

				Invoker->ExecuteOther(ExecuteTarget, TargetLocation, TargetRotation, ExecuteAnimData->AttackMontage, ExecuteAnimData->BeAttackedMontage);
				return true;
			}
		}
	}

	//考虑背刺
	if (ExecuteTarget->CanBeBackstab(Invoker))
	{
		if (const FBackstabAnimData* AttackMontagePair = FindBackstabAnimData(ExecuteTarget))
		{
			if (AttackMontagePair->AttackMontage && AttackMontagePair->BeAttackedMontage)
			{
				FVector TargetLocation(Invoker->GetActorLocation() - Invoker->GetActorRotation().RotateVector(AttackMontagePair->RelationLocation));
				TargetLocation.Z = Invoker->GetActorLocation().Z;
				FRotator TargetRotation(Invoker->GetActorRotation().Pitch, Invoker->GetActorRotation().Yaw, Invoker->GetActorRotation().Roll);

				Invoker->ExecuteOther(ExecuteTarget, TargetLocation, TargetRotation, AttackMontagePair->AttackMontage, AttackMontagePair->BeAttackedMontage);
				return true;
			}
		}
	}

	return false;
}

bool FExecuteActionSet::TraceForExecuteOther(class ACharacterBase* Invoker)
{
	if (Invoker)
	{
		const float TraceDistance = 100.f;
		FHitResult TraceCharacterResult;
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(Invoker, Invoker->GetActorLocation(), Invoker->GetActorLocation() + Invoker->GetActorForwardVector() * TraceDistance, 15.f, { FARPG_CollisionObjectType::Pawn }, false, { Invoker }, EDrawDebugTrace::None, TraceCharacterResult, false))
		{
			FHitResult CanExecuteCheckResult;
			UKismetSystemLibrary::LineTraceSingle(Invoker, Invoker->GetActorLocation(), TraceCharacterResult.GetActor()->GetActorLocation(), FARPG_TraceQueryType::Visibility, false, { Invoker }, EDrawDebugTrace::None, CanExecuteCheckResult, false);
			if (TraceCharacterResult.GetActor() == CanExecuteCheckResult.GetActor())
			{
				if (ACharacterBase* ExecuteTarget = Cast<ACharacterBase>(TraceCharacterResult.GetActor()))
				{
					return InvokeExecuteOther(Invoker, ExecuteTarget);
				}
			}
		}
	}
	return false;
}

const FBackstabAnimData* FExecuteActionSet::FindBackstabAnimData(class ACharacterBase* Character) const
{
	for (UClass* Class = Character->GetClass(); Class != ACharacterBase::StaticClass(); Class = Class->GetSuperClass())
	{
		if (const FBackstabAnimData* BackstabAnimData = BackstabMap.Find(Class))
		{
			return BackstabAnimData;
		}
	}
	return nullptr;
}
