// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AttackAreaManager.h"
#include "CharacterBase.h"

// Sets default values for this component's properties
UARPG_AttackAreaManager::UARPG_AttackAreaManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UARPG_AttackAreaManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
	{
		Character->BattleControl = this;
	}
}


// Called every frame
void UARPG_AttackAreaManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UARPG_AttackAreaManager::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{

}

void UARPG_AttackAreaManager::AbortAttack_Implementation(class AActor* AttackTarget)
{

}

void UARPG_AttackAreaManager::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{

}

