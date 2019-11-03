// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterStateBase.h"
#include <UnrealNetwork.h>
#include <Engine/BlueprintGeneratedClass.h>

UWorld* UARPG_CharacterStateBase::GetWorld() const
{
#if WITH_EDITOR
	if (IsTemplate())
	{
		return nullptr;
	}
#endif
	return GetOuter()->GetWorld();
}

void UARPG_CharacterStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

void UARPG_CharacterState_BuffBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPG_CharacterState_BuffBase, ContinuedTime);
}
