// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XD_SaveGameInterface.h"
#include "ARPG_PropertyDef.h"
#include "ARPG_CharacterStateComponent.generated.h"

class UARPG_CharacterStateBase;
class UARPG_CharacterState_BuffBase;
class UARPG_CharacterState_AccumulationBase;

UCLASS()
class ARPG_API UARPG_CharacterStateComponent : public UActorComponent,
	public IXD_SaveGameInterface
{
	GENERATED_BODY()

public:	
	UARPG_CharacterStateComponent();

	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool NeedSave_Implementation() const override { return true; }
	void WhenPostLoad_Implementation() override;

	void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
public:
	TArray<UARPG_CharacterState_BuffBase*> PrevActivedBuffes;
	UPROPERTY(SaveGame, ReplicatedUsing = OnRep_ActivedBuffes, VisibleAnywhere)
	TArray<UARPG_CharacterState_BuffBase*> ActivedBuffes;
	UFUNCTION()
	void OnRep_ActivedBuffes();

	UFUNCTION(BlueprintCallable)
	void TryAddBuffByType(TSubclassOf<UARPG_CharacterState_BuffBase> BuffType, const FARPG_PropertyChangeContext& ChangeContext);
	UFUNCTION(BlueprintCallable)
	void RemoveBuffByType(TSubclassOf<UARPG_CharacterState_BuffBase> BuffType);

	void AddBuffByRef(UARPG_CharacterState_BuffBase* BuffInstance, const FARPG_PropertyChangeContext& ChangeContext);
public:
	TArray<UARPG_CharacterState_AccumulationBase*> PrevActivedAccumulations;
	UPROPERTY(SaveGame, ReplicatedUsing = OnRep_ActivedAccumulations, VisibleAnywhere)
	TArray<UARPG_CharacterState_AccumulationBase*> ActivedAccumulations;
	UFUNCTION()
	void OnRep_ActivedAccumulations();

	UFUNCTION(BlueprintCallable)
	void AddAccumulation(TSubclassOf<UARPG_CharacterState_AccumulationBase> AccumulationType, float AddAccumulationValue);
};
