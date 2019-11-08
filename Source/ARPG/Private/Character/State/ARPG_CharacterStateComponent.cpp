// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterStateComponent.h"
#include <UnrealNetwork.h>
#include <Engine/ActorChannel.h>

#include "ARPG_CharacterStateBase.h"
#include "CharacterBase.h"

// Sets default values for this component's properties
UARPG_CharacterStateComponent::UARPG_CharacterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetIsReplicated(true);
}


// Called when the game starts
void UARPG_CharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARPG_CharacterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for (UARPG_CharacterState_BuffBase* Buff : ActivedBuffes)
	{
		const float NextContinuedTime = Buff->ContinuedTime + DeltaTime;

		const float IntervalTime = Buff->IntervalTime;
		if (IntervalTime == 0.f)
		{
			Buff->Tick(DeltaTime);
		}
		else if (FMath::Fmod(Buff->ContinuedTime, IntervalTime) > FMath::Fmod(NextContinuedTime, IntervalTime))
		{
			Buff->Tick(IntervalTime);
		}

		Buff->ContinuedTime = NextContinuedTime;

		if (Buff->DurationTime > 0.f && Buff->DurationTime < NextContinuedTime)
		{
			Buff->Deactive();
		}
	}
	ActivedBuffes.RemoveAll([](UARPG_CharacterState_BuffBase* E) {return E->DurationTime > 0.f && E->ContinuedTime > E->DurationTime; });

	for (int32 Idx = 0; Idx < ActivedAccumulations.Num();)
	{
		UARPG_CharacterState_AccumulationBase* Accumulation = ActivedAccumulations[Idx];
		Accumulation->AccumulationValue -= DeltaTime;
		if (Accumulation->AccumulationValue <= 0.f)
		{
			ActivedAccumulations.RemoveAt(Idx);
		}
		else
		{
			++Idx;
		}
	}
}

void UARPG_CharacterStateComponent::WhenPostLoad_Implementation()
{
	ACharacterBase* Owner = CastChecked<ACharacterBase>(GetOwner());
	for (UARPG_CharacterState_BuffBase* Buff : ActivedBuffes)
	{
		Buff->Owner = Owner;
		Buff->Active(false);
	}

	for (UARPG_CharacterState_AccumulationBase* Accumulation : ActivedAccumulations)
	{
		Accumulation->Owner = Owner;
	}
}

void UARPG_CharacterStateComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPG_CharacterStateComponent, ActivedBuffes);
	DOREPLIFETIME(UARPG_CharacterStateComponent, ActivedAccumulations);
}

bool UARPG_CharacterStateComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool IsFailed = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UARPG_CharacterState_BuffBase* Buff : ActivedBuffes)
	{
		if (Buff)
		{
			IsFailed |= Channel->ReplicateSubobject(Buff, *Bunch, *RepFlags);
		}
	}

	for (UARPG_CharacterState_AccumulationBase* Accumulation : ActivedAccumulations)
	{
		if (Accumulation)
		{
			IsFailed |= Channel->ReplicateSubobject(Accumulation, *Bunch, *RepFlags);
		}
	}

	return IsFailed;
}

void UARPG_CharacterStateComponent::OnRep_ActivedBuffes()
{
	ACharacterBase* Owner = CastChecked<ACharacterBase>(GetOwner());
	for (UARPG_CharacterState_BuffBase* AddBuff : TSet<UARPG_CharacterState_BuffBase*>(ActivedBuffes).Difference(TSet<UARPG_CharacterState_BuffBase*>(PrevActivedBuffes)))
	{
		if (AddBuff)
		{
			AddBuff->Owner = Owner;
			AddBuff->Active(true);
		}
	}

	for (UARPG_CharacterState_BuffBase* RemoveBuff : TSet<UARPG_CharacterState_BuffBase*>(PrevActivedBuffes).Difference(TSet<UARPG_CharacterState_BuffBase*>(ActivedBuffes)))
	{
		if (RemoveBuff)
		{
			RemoveBuff->Deactive();
		}
	}

	PrevActivedBuffes = ActivedBuffes;
}

void UARPG_CharacterStateComponent::TryAddBuffByType(TSubclassOf<UARPG_CharacterState_BuffBase> BuffType, const FARPG_PropertyChangeContext& ChangeContext)
{
	if (GetOwner()->HasAuthority() && BuffType)
	{
		const bool bAllowMulitSameTypeBuff = BuffType.GetDefaultObject()->bAllowMulitSameTypeBuff;
		if (bAllowMulitSameTypeBuff == false)
		{
			int32 FindIdx = ActivedBuffes.IndexOfByPredicate([&](const UARPG_CharacterState_BuffBase* E) {return E->GetClass() == BuffType; });
			if (FindIdx != INDEX_NONE)
			{
				ActivedBuffes[FindIdx]->RepeatActive();
				return;
			}
		}
		UARPG_CharacterState_BuffBase* Buff = NewObject<UARPG_CharacterState_BuffBase>(this, BuffType);
		AddBuffByRef(Buff, ChangeContext);
	}
}

void UARPG_CharacterStateComponent::RemoveBuffByType(TSubclassOf<UARPG_CharacterState_BuffBase> BuffType)
{
	if (GetOwner()->HasAuthority() && BuffType)
	{
		for (UARPG_CharacterState_BuffBase* ActivedBuff : ActivedBuffes)
		{
			if (ActivedBuff->IsA(BuffType))
			{
				ActivedBuff->Deactive();
			}
		}
		ActivedBuffes.RemoveAll([&](UARPG_CharacterState_BuffBase* E) {return E->IsA(BuffType); });
	}
}

void UARPG_CharacterStateComponent::AddBuffByRef(UARPG_CharacterState_BuffBase* BuffInstance, const FARPG_PropertyChangeContext& ChangeContext)
{
	BuffInstance->Owner = CastChecked<ACharacterBase>(GetOwner());
	BuffInstance->Instigator = ChangeContext.Instigator;
	BuffInstance->Active(true);
	ActivedBuffes.Add(BuffInstance);
}

void UARPG_CharacterStateComponent::OnRep_ActivedAccumulations()
{
	ACharacterBase* Owner = CastChecked<ACharacterBase>(GetOwner());
	for (UARPG_CharacterState_AccumulationBase* Accumulation : TSet<UARPG_CharacterState_AccumulationBase*>(ActivedAccumulations).Difference(TSet<UARPG_CharacterState_AccumulationBase*>(PrevActivedAccumulations)))
	{
		if (Accumulation)
		{
			Accumulation->Owner = Owner;
		}
	}

	for (UARPG_CharacterState_AccumulationBase* Accumulation : TSet<UARPG_CharacterState_AccumulationBase*>(PrevActivedAccumulations).Difference(TSet<UARPG_CharacterState_AccumulationBase*>(ActivedAccumulations)))
	{
		if (Accumulation)
		{

		}
	}

	PrevActivedAccumulations = ActivedAccumulations;
}

void UARPG_CharacterStateComponent::AddAccumulation(TSubclassOf<UARPG_CharacterState_AccumulationBase> AccumulationType, float AddAccumulationValue)
{
	int32 Idx = ActivedAccumulations.IndexOfByPredicate([&](const UARPG_CharacterState_AccumulationBase* E) {return E->GetClass() == AccumulationType; });

	UARPG_CharacterState_AccumulationBase* Accumulation;
	if (Idx == INDEX_NONE)
	{
		Accumulation = NewObject<UARPG_CharacterState_AccumulationBase>(this, AccumulationType);
		Accumulation->Owner = CastChecked<ACharacterBase>(GetOwner());
		Accumulation->AccumulationValue = AddAccumulationValue;
		ActivedAccumulations.Add(Accumulation);
	}
	else
	{
		Accumulation = ActivedAccumulations[Idx];
		Accumulation->AccumulationValue += AddAccumulationValue;
	}

	if (Accumulation->AccumulationValue >= Accumulation->GetOverflowValue())
	{
		ActivedAccumulations.Remove(Accumulation);
		Accumulation->Overflow();
	}
}
