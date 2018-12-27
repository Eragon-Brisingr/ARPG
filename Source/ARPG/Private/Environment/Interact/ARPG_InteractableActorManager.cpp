// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_InteractableActorManager.h"
#include "CharacterBase.h"
#include "ARPG_MoveUtils.h"
#include "ARPG_CharacterBehaviorBase.h"
#include "ARPG_ActorFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Action/ARPG_EnterReleaseStateBase.h"

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
		if (bForceEnterReleaseState && Invoker->IsInReleaseState() == false && Invoker->EnterReleaseStateAction)
		{
			Invoker->EnterReleaseState({});
		}

		const FInteractBehaviorConfig* ValidConfig = nullptr;
		const FInteractBehavior* ValidBehavior = nullptr;
		GetBehavior(Invoker, ValidConfig, ValidBehavior);
		if (ValidConfig && ValidBehavior)
		{
			FVector WorldLocation = ValidBehavior->GetWorldLocation(this);
			UARPG_MoveUtils::ARPG_MoveToLocation(Invoker, WorldLocation, FOnARPG_MoveFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenMoveFinished, Invoker, ValidConfig, ValidBehavior,
				FOnInteractFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenInteractFinished, Invoker, OnInteractFinished)), 1.f);
			return;
		}
	}
	OnInteractFinished.ExecuteIfBound(false);
}

void UARPG_InteractableActorManagerBase::WhenMoveFinished(const FPathFollowingResult& Result, ACharacterBase* Invoker, const FInteractBehaviorConfig* InvokeConfig, const FInteractBehavior* InvokeBehavior, FOnInteractFinished OnInteractFinished)
{
	//TODO 考虑动态物体 计算距离 (InvokeBehavior->GetWorldLocation(this) - Invoker->GetActorLocation())
	if (InvokeConfig->User == nullptr && Result.Code == EPathFollowingResult::Success)
	{
		if (bForceEnterReleaseState && Invoker->IsInReleaseState() == false && Invoker->EnterReleaseStateAction)
		{
			FOnInteractFinished FOnEnterReleaseState = FOnInteractFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenEnterReleaseState, Invoker, InvokeConfig, InvokeBehavior, OnInteractFinished);
			if (Invoker->EnterReleaseStateAction->bIsExecuting)
			{
				Invoker->EnterReleaseStateAction->OnBehaviorFinished = FOnEnterReleaseState;
			}
			else
			{
				CurBehaviorMap.FindOrAdd(Invoker) = Invoker->EnterReleaseState(FOnEnterReleaseState);
			}
		}
		else
		{
			WhenEnterReleaseState(true, Invoker, InvokeConfig, InvokeBehavior, OnInteractFinished);
		}
	}
	else
	{
		OnInteractFinished.ExecuteIfBound(false);
	}
}

void UARPG_InteractableActorManagerBase::WhenTurnFinished(bool Succeed, ACharacterBase* Invoker, FInteractBehavior BehaviorConfig, FOnInteractFinished OnInteractFinished)
{
	UARPG_CharacterBehaviorConfigurable* Behavior = BehaviorConfig.RelativePositionExecuteBehavior(Invoker, OnInteractFinished, GetOwner());
	CurBehaviorMap.FindOrAdd(Invoker) = Behavior;
}

void UARPG_InteractableActorManagerBase::WhenInteractFinished(bool Succeed, ACharacterBase* Invoker, FOnInteractFinished OnInteractFinished)
{
	OnInteractFinished.ExecuteIfBound(Succeed);
	ExecuteWhenEndInteract(Invoker, Succeed);
}

void UARPG_InteractableActorManagerBase::WhenEnterReleaseState(bool Succeed, ACharacterBase* Invoker, const FInteractBehaviorConfig* InvokeConfig, const FInteractBehavior* InvokeBehavior, FOnInteractFinished OnInteractFinished)
{
	if (Succeed && CanInteract(Invoker))
	{
		const FInteractBehavior& Behavior = *InvokeBehavior;
		if (Behavior.Behavior)
		{
			InteractActorBeginSetCollision(Invoker);
			ExecuteWhenBeginInteract(Invoker, const_cast<FInteractBehaviorConfig&>(*InvokeConfig));
			if (Behavior.bAttachToRotation && Invoker->CharacterTurnAction && Invoker->CanPlayTurnMontage())
			{
				FTransform Transfrom = GetOwner()->GetActorTransform();
				if (Behavior.bAttachToLocation)
				{
					UARPG_ActorFunctionLibrary::MoveCharacterToLocationFitGround(Invoker, Transfrom.TransformPosition(Behavior.Location), 1.f);
				}
				if (UARPG_CharacterBehaviorBase* TurnBehavior = Invoker->TurnTo(Transfrom.TransformRotation(Behavior.Rotation.Quaternion()).Rotator(), FOnCharacterBehaviorFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenTurnFinished, Invoker, Behavior, OnInteractFinished)))
				{
					CurBehaviorMap.FindOrAdd(Invoker) = TurnBehavior;
				}
			}
			else
			{
				WhenTurnFinished(true, Invoker, Behavior, OnInteractFinished);
			}
			return;
		}
	}
	else
	{
		OnInteractFinished.ExecuteIfBound(false);
	}
}

void UARPG_InteractableActorManagerBase::EndInteract(ACharacterBase* Invoker, const FOnInteractAbortFinished& OnInteractAbortFinished)
{
	Invoker->StopMovement();
	if (UARPG_CharacterBehaviorBase** P_Behavior = CurBehaviorMap.Find(Invoker))
	{
		(*P_Behavior)->AbortBehavior(Invoker, FOnCharacterBehaviorAbortFinished::CreateUObject(this, &UARPG_InteractableActorManagerBase::WhenBehaviorAbortFinished, Invoker, OnInteractAbortFinished));
		CurBehaviorMap.Remove(Invoker);
	}
	else
	{
		OnInteractAbortFinished.ExecuteIfBound();
		ExecuteWhenEndInteract(Invoker, false);
	}
}

void UARPG_InteractableActorManagerBase::WhenBehaviorAbortFinished(ACharacterBase* Invoker, FOnInteractAbortFinished OnInteractAbortFinished)
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
