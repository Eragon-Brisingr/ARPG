// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActor.h"
#include "ARPG_AD_InteractableBase.h"
#include "ARPG_MoveUtils.h"
#include "CharacterBase.h"
#include "XD_DebugFunctionLibrary.h"
#include "ARPG_AI_Log.h"

void FARPG_InteractSingleConfig::InitConfig(AActor* Owner)
{
	if (InteractDispatcher)
	{
		InteractDispatcher->InitInteractDispatcher(Owner);
	}
}

bool FARPG_InteractSingleConfig::CanInteract(const ACharacterBase* InteractInvoker) const
{
	return InteractDispatcher && InteractDispatcher->State == EActionDispatcherState::Deactive;
}

void FARPG_InteractSingleConfig::WhenInvokeInteract(AActor* Owner, ACharacterBase* InteractInvoker)
{
	FVector WorldLocation = Owner->GetActorTransform().TransformPosition(InteractLocation.GetLocation());
	AI_Display_VLog(Owner, "%s申请与%s交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractInvoker), *UXD_DebugFunctionLibrary::GetDebugName(Owner));
	FOnARPG_MoveFinished OnARPG_MoveFinished = FOnARPG_MoveFinished::CreateWeakLambda(Owner, [=](const FPathFollowingResult & Result)
		{
			if (Result.Code == EPathFollowingResult::Success)
			{
				if (IARPG_InteractInterface::CanInteract(Owner, InteractInvoker))
				{
					AI_Display_VLog(Owner, "%s开始与%s交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractInvoker), *UXD_DebugFunctionLibrary::GetDebugName(Owner));
					InteractInvoker->SetInteractingTarget(Owner);
					InteractDispatcher->StartInteractDispatcher(InteractInvoker,
						FOnDispatchDeactiveNative::CreateWeakLambda(Owner, [=](bool IsFinishedCompleted)
							{
								AI_Display_VLog(Owner, "%s结束与%s交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractInvoker), *UXD_DebugFunctionLibrary::GetDebugName(Owner));
								InteractInvoker->ExecuteInteractEnd(IsFinishedCompleted ? EInteractEndResult::Succeed : EInteractEndResult::Failed);
							}));
				}
				else
				{
					InteractInvoker->ExecuteInteractEnd(EInteractEndResult::CanNotInteract);
				}
			}
			else
			{
				InteractInvoker->ExecuteInteractEnd(EInteractEndResult::CanNotInteract);
			}
		});

	if (bSnapRotation)
	{
		FRotator WorldRotation = Owner->GetActorTransform().TransformRotation(InteractLocation.GetRotation()).Rotator();
		UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(InteractInvoker, WorldLocation, WorldRotation, OnARPG_MoveFinished, StartBehaviousRadius);
	}
	else
	{
		UARPG_MoveUtils::ARPG_MoveToLocation(InteractInvoker, WorldLocation, OnARPG_MoveFinished, StartBehaviousRadius);
	}
}

void FARPG_InteractSingleConfig::WhenAbortInteract(AActor* Owner, ACharacterBase* InteractInvoker)
{
	if (InteractInvoker == GetInteracter())
	{
		if (InteractDispatcher->State == EActionDispatcherState::Active)
		{
			AI_Display_VLog(InteractInvoker, "%s中断与%s交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractInvoker), *UXD_DebugFunctionLibrary::GetDebugName(Owner));
			InteractDispatcher->AbortInteractDispatcher(FOnDispatcherAbortedNative::CreateWeakLambda(Owner, [=]()
				{
					InteractInvoker->ExecuteInteractAbortEnd();
				}));
		}
	}
	else
	{
		AI_Display_VLog(InteractInvoker, "%s中断申请与%s交互", *UXD_DebugFunctionLibrary::GetDebugName(InteractInvoker), *UXD_DebugFunctionLibrary::GetDebugName(Owner));
		InteractInvoker->StopMovement();
		InteractInvoker->ExecuteInteractAbortEnd();
	}
}

ACharacterBase* FARPG_InteractSingleConfig::GetInteracter() const
{
	return InteractDispatcher ? InteractDispatcher->InteractInvoker.Get() : nullptr;
}

AARPG_InteractableActorSingle::AARPG_InteractableActorSingle()
{

}

ACharacterBase* AARPG_InteractableActorSingle::GetInteracter() const
{
	return InteractSingleConfig.GetInteracter();
}

bool AARPG_InteractableActorSingle::CanInteract_Implementation(const ACharacterBase* InteractInvoker) const
{
	return InteractSingleConfig.CanInteract(InteractInvoker);
}

void AARPG_InteractableActorSingle::WhenInvokeInteract_Implementation(ACharacterBase* InteractInvoker)
{
	InteractSingleConfig.WhenInvokeInteract(this, InteractInvoker);
}

void AARPG_InteractableActorSingle::WhenAbortInteract_Implementation(ACharacterBase* InteractInvoker)
{
	InteractSingleConfig.WhenAbortInteract(this, InteractInvoker);
}

void AARPG_InteractableActorSingle::BeginPlay()
{
	Super::BeginPlay();
	InteractSingleConfig.InitConfig(this);
}

#if WITH_EDITOR
void AARPG_InteractableActorSingle::DrawGizmoNative(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI, bool IsSelected)
{
	DrawDirectionalArrow(PDI, (InteractSingleConfig.InteractLocation * GetActorTransform()).ToMatrixWithScale(), FColor::Red, 50.f, 5.f, SDPG_World, 1.f);
}
#endif
