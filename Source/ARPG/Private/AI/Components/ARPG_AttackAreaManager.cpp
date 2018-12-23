// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AttackAreaManager.h"
#include "CharacterBase.h"

UWorld* UARPG_AttackParamBase::GetWorld() const
{
	return Attacker ? Attacker->GetWorld() : nullptr;
}

FVector UARPG_AttackArea_Sphere::GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const
{
	FVector TargetLocation = AttackTarget->GetActorLocation();
	return TargetLocation + (Attacker->GetActorLocation() - TargetLocation).GetSafeNormal2D() * GetWorldRadius(Attacker->GetActorScale()) * 0.5f;
}

bool UARPG_AttackArea_Sphere::IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const
{
	return Attacker->GetDistanceTo(AttackTarget) < GetWorldRadius(Attacker->GetActorScale());
}

float UARPG_AttackArea_Sphere::GetWorldRadius(const FVector& OwnerWorldScale) const
{
	return OwnerWorldScale.Z * Radius;
}

FVector UARPG_AttackArea_Box::GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const
{
	FTransform WorldTransform = Attacker->GetActorTransform();
	FVector WorldExtents = WorldTransform.TransformVector(Extent);
	return WorldTransform.GetLocation() + (Attacker->GetActorLocation() - WorldTransform.GetLocation()).GetSafeNormal2D() * WorldExtents.GetMin() * 0.5f;
}

bool UARPG_AttackArea_Box::IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const
{
	FTransform WorldTransform = Attacker->GetActorTransform();

	return true;
}

void UAP_NormalMontage::InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	Attacker->PlayMontageWithBlendingOutDelegate(Montage, FOnMontageBlendingOutStarted::CreateUObject(this, &UAP_NormalMontage::WhenMontageBlendOutStart, OnAttackFinished));
}

void UAP_NormalMontage::WhenMontageBlendOutStart(UAnimMontage* CurMontage, bool bInterrupted, FBP_OnAttackFinished OnAttackFinished)
{
	OnAttackFinished.ExecuteIfBound(!bInterrupted);
}

// Sets default values for this component's properties
UARPG_AttackAreaManager::UARPG_AttackAreaManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bAutoActivate = true;
}


// Called when the game starts
void UARPG_AttackAreaManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ACharacterBase* Character = Cast<ACharacterBase>(GetOwner()))
	{
		Attacker = Character;
		if (bAutoActivate)
		{
			Character->BattleControl = this;
		}

		for (FAttackAreaParam& AttackAreaParam : AttackConfigs)
		{
			if (AttackAreaParam.AttackParam)
			{
				AttackAreaParam.AttackParam->Attacker = Character;
			}
		}
	}

}


// Called every frame
void UARPG_AttackAreaManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UARPG_AttackAreaManager::GetAttackMoveLocation_Implementation(class AActor* AttackTarget) const
{
	if (UARPG_AttackAreaBase* AttackArea = AttackConfigs[MainAttackConfig].AttackArea)
	{
		return AttackConfigs[MainAttackConfig].AttackArea->GetAttackMoveLocation(Attacker, AttackTarget);
	}

	return FVector::ZeroVector;
}

void UARPG_AttackAreaManager::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	for (int32 i = 0; i < AttackConfigs.Num(); ++i)
	{
		const FAttackAreaParam& AttackAreaParam = AttackConfigs[i];
		if (AttackAreaParam.AttackParam)
		{
			ActiveAttackConfigIndex = i;
			AttackAreaParam.AttackParam->InvokeAttack(AttackTarget, FARPG_OnAttackFinished::CreateUObject(this, &UARPG_AttackAreaManager::WhenAttackFinished, OnAttackFinished));
		}
	}
}

void UARPG_AttackAreaManager::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{
	if (UARPG_AttackParamBase* AttackParam = GetActiveAttackParam())
	{
		AttackParam->AbortAttack(AttackTarget, OnAttackAborted);
	}
}

void UARPG_AttackAreaManager::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{
	if (UARPG_AttackParamBase* AttackParam = GetActiveAttackParam())
	{
		AttackParam->AttackingTick(AttackTarget, DeltaSecond);
	}
}

void UARPG_AttackAreaManager::WhenAttackFinished(bool Succeed, FBP_OnAttackFinished OnAttackFinished)
{
	ActiveAttackConfigIndex = INDEX_NONE;
	OnAttackFinished.ExecuteIfBound(Succeed);
}
