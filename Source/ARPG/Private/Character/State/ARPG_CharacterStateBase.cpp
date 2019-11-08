// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_CharacterStateBase.h"
#include <UnrealNetwork.h>
#include <Engine/BlueprintGeneratedClass.h>
#include <CharacterBase.h>

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

inline bool UARPG_CharacterStateBase::HasAuthority() const 
{ 
	return Owner->HasAuthority(); 
}

UARPG_CharacterState_BuffBase::UARPG_CharacterState_BuffBase()
	:bAllowMulitSameTypeBuff(false)
{

}

void UARPG_CharacterState_BuffBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARPG_CharacterState_BuffBase, IntervalTime);
	DOREPLIFETIME(UARPG_CharacterState_BuffBase, DurationTime);
	DOREPLIFETIME(UARPG_CharacterState_BuffBase, ContinuedTime);
}
