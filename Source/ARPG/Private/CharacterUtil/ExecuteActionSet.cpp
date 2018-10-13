// Fill out your copyright notice in the Description page of Project Settings.

#include "ExecuteActionSet.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "CharacterBase.h"



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
				FVector TargetLocation(ExecuteTarget->GetActorLocation() + ExecuteTarget->GetActorRotation().RotateVector(ExecuteAnimData->RelationLocation));
				TargetLocation.Z = Invoker->GetActorLocation().Z;
				FRotator TargetRotation(Invoker->GetActorRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(Invoker->GetActorLocation(), ExecuteTarget->GetActorLocation()).Yaw + ExecuteAnimData->YawOffset, Invoker->GetActorRotation().Roll);

				Invoker->ExecuteOther(ExecuteTarget, TargetLocation, TargetRotation, ExecuteAnimData->AttackMontage, ExecuteAnimData->BeAttackedMontage);
				return true;
			}
		}
	}

	//考虑背刺
	if (ExecuteTarget->CanBeBackstab(Invoker))
	{
		if (const FBackstabAnimData* AttackMontagePair = BackstabMap.Find(ExecuteTarget->GetMesh()->SkeletalMesh->Skeleton))
		{
			if (AttackMontagePair->AttackMontage && AttackMontagePair->BeAttackedMontage)
			{
				FVector TargetLocation(ExecuteTarget->GetActorLocation() + ExecuteTarget->GetActorRotation().RotateVector(AttackMontagePair->RelationLocation));
				TargetLocation.Z = Invoker->GetActorLocation().Z;
				FRotator TargetRotation(Invoker->GetActorRotation().Pitch, ExecuteTarget->GetActorRotation().Yaw, Invoker->GetActorRotation().Roll);

				Invoker->ExecuteOther(ExecuteTarget, TargetLocation, TargetRotation, AttackMontagePair->AttackMontage, AttackMontagePair->BeAttackedMontage);
				return true;
			}
		}
	}

	return false;
}

bool FExecuteActionSet::TraceForExecuteOther(class ACharacterBase* Invoker)
{
	const float TraceDistance = 100.f;
	FHitResult TraceCharacterResult;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(Invoker, Invoker->GetActorLocation(), Invoker->GetActorLocation() + Invoker->GetActorForwardVector() * TraceDistance, 15.f, { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) }, false, { Invoker }, EDrawDebugTrace::ForDuration, TraceCharacterResult, false))
	{
		FHitResult CanExecuteCheckResult;
		UKismetSystemLibrary::LineTraceSingle(Invoker, Invoker->GetActorLocation(), TraceCharacterResult.GetActor()->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), false, { Invoker }, EDrawDebugTrace::ForDuration, CanExecuteCheckResult, false);
		if (TraceCharacterResult.GetActor() == CanExecuteCheckResult.GetActor())
		{
			if (ACharacterBase* ExecuteTarget = Cast<ACharacterBase>(TraceCharacterResult.GetActor()))
			{
				return InvokeExecuteOther(Invoker, ExecuteTarget);
			}
		}
	}
	return false;
}
