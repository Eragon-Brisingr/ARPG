// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActorManager.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "ARPG_AD_InteractableBase.h"
#include "XD_DispatchableActionBase.h"

// Sets default values for this component's properties
UARPG_InteractableActorManagerBase::UARPG_InteractableActorManagerBase()
	:bForceEnterReleaseState(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UARPG_InteractableActorManagerBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARPG_InteractableActorManagerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARPG_InteractableActorManagerBase::StartInteract(ACharacterBase* Invoker, const FOnInteractFinished& OnInteractFinished)
{
	if (CanInteract(Invoker))
	{
		const FInteractBehaviorConfig* ValidConfig = nullptr;
		const FInteractBehavior* ValidBehavior = nullptr;
		GetBehavior(Invoker, ValidConfig, ValidBehavior);

		// TODO 之后修改为CreateWeakLamdba
		TWeakObjectPtr<UARPG_InteractableActorManagerBase> WeakSelf = this;
		auto MoveToAndInteract = [=]()
		{
			auto OnMoveFinished = FOnARPG_MoveFinished::CreateLambda([=](const FPathFollowingResult& Result)
			{
				if (WeakSelf.IsValid() == false)
				{
					return;
				}

				//TODO 考虑动态物体 计算距离 (InvokeBehavior->GetWorldLocation(this) - Invoker->GetActorLocation())
				if (ValidConfig->User == nullptr && Result.Code == EPathFollowingResult::Success)
				{
					StartInteractImpl(Invoker, ValidBehavior, ValidConfig, OnInteractFinished);
				}
				else
				{
					OnInteractFinished.ExecuteIfBound(EInteractResult::CanNotInteract);
				}
			});

			if (ValidConfig && ValidBehavior)
			{
				FVector WorldLocation = ValidBehavior->GetWorldLocation(this);
				if (ValidBehavior->bAttachToRotation)
				{
					FRotator WorldRotation = ValidBehavior->GetWorldRotation(this);
					UARPG_MoveUtils::ARPG_MoveToLocationAndTurn(Invoker, WorldLocation, WorldRotation, OnMoveFinished, 0.f);
				}
				else
				{
					UARPG_MoveUtils::ARPG_MoveToLocation(Invoker, WorldLocation, OnMoveFinished);
				}
				return;
			}
		};

		if (bForceEnterReleaseState && Invoker->IsInReleaseState() == false && Invoker->EnterReleaseStateAction)
		{
			Invoker->EnterReleaseState(FOnCharacterBehaviorFinished::CreateLambda([=](bool Succeed)
			{
				if (WeakSelf.IsValid() == false)
				{
					return;
				}

				if (Succeed)
				{
					MoveToAndInteract();
				}
				else
				{
					OnInteractFinished.ExecuteIfBound(EInteractResult::CanNotInteract);
				}
			}));
		}
		else
		{
			MoveToAndInteract();
		}
	}
	else
	{
		OnInteractFinished.ExecuteIfBound(EInteractResult::CanNotInteract);
	}
}

void UARPG_InteractableActorManagerBase::StartInteractImpl(ACharacterBase* Invoker, const FInteractBehavior* InvokeBehavior, const FInteractBehaviorConfig* InvokeConfig, const FOnInteractFinished& OnInteractFinished)
{
	const FInteractBehavior& BehaviorData = *InvokeBehavior;
	UARPG_AD_InteractableBase* Behavior = BehaviorData.InteractAction;
	if (Behavior)
	{
		InteractActorBeginSetCollision(Invoker);
		ExecuteWhenBeginInteract(Invoker, const_cast<FInteractBehaviorConfig&>(*InvokeConfig));

		if (BehaviorData.bAttachToActor)
		{
			Invoker->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
		}

		Behavior->StartInteractDispatcher(GetOwner(), Invoker, FWhenDispatchFinishedNative::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenInteractFinishedSucceed, Invoker, OnInteractFinished),
			FOnActionDispatcherAbortedNative::FDelegate::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenInteractFinishedFailed, Invoker, OnInteractFinished));

		CurBehaviorMap.FindOrAdd(Invoker) = Behavior;
		return;
	}
}

void UARPG_InteractableActorManagerBase::WhenInteractFinishedSucceed(const FName& Tag, ACharacterBase* Invoker, FOnInteractFinished OnInteractFinished)
{
	//TODO 把Tag传出去
	OnInteractFinished.ExecuteIfBound(EInteractResult::InteractedSucceed);
	ExecuteWhenEndInteract(Invoker, true);
}

void UARPG_InteractableActorManagerBase::WhenInteractFinishedFailed(ACharacterBase* Invoker, FOnInteractFinished OnInteractFinished)
{
	OnInteractFinished.ExecuteIfBound(EInteractResult::InteractedFailed);
	ExecuteWhenEndInteract(Invoker, false);
}

void UARPG_InteractableActorManagerBase::EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished)
{
	Invoker->StopMovement();
	if (UARPG_AD_InteractableBase** P_Behavior = CurBehaviorMap.Find(Invoker))
	{
		UXD_DispatchableActionBase* Action = Invoker->CurrentAction.Get();
		if (Action->GetOwner() == (*P_Behavior))
		{
			Action->OnActionAborted.BindUObject(this, &UARPG_InteractableActorManagerBase::WhenActionAbortFinished, Invoker, OnInteractAbortFinished);
			Action->AbortDispatcher({});
		}
		else
		{
			(*P_Behavior)->AbortInteractDispatcher(FOnActionDispatcherAbortedNative::FDelegate::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenActionAbortFinished, Invoker, OnInteractAbortFinished));
		}
		CurBehaviorMap.Remove(Invoker);
	}
	else
	{
		OnInteractAbortFinished.ExecuteIfBound();
		ExecuteWhenEndInteract(Invoker, false);
	}
}

void UARPG_InteractableActorManagerBase::WhenActionAbortFinished(ACharacterBase* Invoker, FOnInteractAbortFinished OnInteractAbortFinished)
{
	OnInteractAbortFinished.ExecuteIfBound();
	ExecuteWhenEndInteract(Invoker, false);
}

void UARPG_InteractableActorManagerBase::InteractActorBeginSetCollision(ACharacterBase* Invoker)
{
	if (bCancelCapsuleCollision)
	{
		Invoker->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UARPG_InteractableActorManagerBase::InteractActorEndSetCollision(ACharacterBase* Invoker)
{
	if (bCancelCapsuleCollision)
	{
		Invoker->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void UARPG_InteractableActorManagerBase::ExecuteWhenBeginInteract(ACharacterBase* Invoker, FInteractBehaviorConfig& InvokeConfig)
{
	Invoker->InteractingManager = this;
	Invoker->bIsInteractingWithActor = true;
	InvokeConfig.User = Invoker;
	WhenBeginInteract(Invoker, InvokeConfig);
	OnInteractBegin.Broadcast(GetOwner(), this, Invoker);
}

void UARPG_InteractableActorManagerBase::ExecuteWhenEndInteract(ACharacterBase* Invoker, bool bFinishPerfectly)
{
	if (Invoker->InteractingManager == this)
	{
		CurBehaviorMap.Remove(Invoker);
		InteractActorEndSetCollision(Invoker);
		Invoker->InteractingManager = nullptr;
		Invoker->bIsInteractingWithActor = false;
		WhenEndInteract(Invoker);
		OnInteractEnd.Broadcast(GetOwner(), this, Invoker, bFinishPerfectly);

		if (Invoker->IsAttachedTo(GetOwner()))
		{
			Invoker->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}
}

UInteractableActorManagerSingle::UInteractableActorManagerSingle()
{
	
}

const FInteractBehavior* GetBehaviorByNearestLocation(const TArray<FInteractBehavior>& Behaviors, const FTransform& Transform, ACharacterBase* Invoker)
{
	if (Behaviors.Num() > 0)
	{
		const FInteractBehavior* NearestBehavior = &Behaviors[0];
		float Distance = (Invoker->GetActorLocation() - Transform.TransformPosition(NearestBehavior->Location)).Size();
		for (int i = 1; i < Behaviors.Num(); ++i)
		{
			float CompareDistance = (Invoker->GetActorLocation() - Transform.TransformPosition(Behaviors[i].Location)).Size();
			if (Distance > CompareDistance)
			{
				CompareDistance = Distance;
				NearestBehavior = &Behaviors[i];
			}
		}
		return NearestBehavior;
	}
	return nullptr;
}

void UInteractableActorManagerSingle::GetBehavior(ACharacterBase* Invoker, const FInteractBehaviorConfig*& ValidConfig, const FInteractBehavior*& ValidBehavior) const
{
	ValidBehavior = GetBehaviorByNearestLocation(GetBehaviors(), GetOwner()->GetTransform(), Invoker);
	ValidConfig = &Config;
}

void UInteractableActorManagerSingle::WhenEndInteract(ACharacterBase* Invoker)
{
	Config.User = nullptr;
}

UInteractableActorManagerMulti::UInteractableActorManagerMulti()
{

}

bool UInteractableActorManagerMulti::CanInteract(const ACharacterBase* Invoker) const
{
	return Configs.ContainsByPredicate([](const FInteractBehaviorConfig& InteractBehaviorConfig) {return !::IsValid(InteractBehaviorConfig.User); });
}

void UInteractableActorManagerMulti::GetBehavior(ACharacterBase* Invoker, const FInteractBehaviorConfig*& ValidConfig, const FInteractBehavior*& ValidBehavior) const
{
	for (const FInteractBehaviorConfig& InteractBehaviorConfig : Configs)
	{
		if (!::IsValid(InteractBehaviorConfig.User))
		{
			ValidBehavior = GetBehaviorByNearestLocation(InteractBehaviorConfig.Behaviors, GetOwner()->GetActorTransform(), Invoker);
			if (ValidBehavior)
			{
				ValidConfig = &InteractBehaviorConfig;
				return;
			}
		}
	}
}

void UInteractableActorManagerMulti::WhenEndInteract(ACharacterBase* Invoker)
{
	if (FInteractBehaviorConfig* FindInteractBehaviorConfig = Configs.FindByPredicate([&](const FInteractBehaviorConfig& InteractBehaviorConfig) {return InteractBehaviorConfig.User == Invoker; }))
	{
		FindInteractBehaviorConfig->User = nullptr;
	}
}
