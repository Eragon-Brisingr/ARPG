// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_ARPG_IsTargetInShape.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

bool UBTD_ARPG_InSphere::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
				FVector SphereLocation = Character->GetActorTransform().TransformPosition(RelativeLocation);
				bool IsInSphere = (SphereLocation - TargetActor->GetActorLocation()).Size() < Radius;
#if WITH_EDITOR
				if (bShowDebugSphere)
				{
					DrawDebugSphere(Character->GetWorld(), SphereLocation, Radius, 32, IsInSphere ? FColor::Green : FColor::Red, false, 2.f);
				}
#endif

				if (IsInSphere)
				{
					return true;
				}
			}
		}
	}
	return false;
}

FString UBTD_ARPG_InSphere::GetStaticDescription() const
{
	return TEXT("目标在球形中");
}

bool UBTD_ARPG_InBox::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
				FTransform OwnerTransform = Character->GetActorTransform();
				FTransform BoxTransform{ OwnerTransform.TransformRotation(RelativeRotation.Quaternion()), OwnerTransform.TransformPosition(RelativeLocation), OwnerTransform.GetScale3D() };
				bool IsInBox = UKismetMathLibrary::IsPointInBoxWithTransform(TargetActor->GetActorLocation(), BoxTransform, BoxExtent);
#if WITH_EDITOR
				if (bShowDebugBox)
				{
					DrawDebugBox(Character->GetWorld(), BoxTransform.GetLocation(), BoxExtent * OwnerTransform.GetScale3D(), BoxTransform.GetRotation(), IsInBox ? FColor::Green : FColor::Red, false, 2.f);
				}
#endif

				if (IsInBox)
				{
					return true;
				}
			}
		}
	}
	return false;
}

FString UBTD_ARPG_InBox::GetStaticDescription() const
{
	return TEXT("目标在矩形中");
}
