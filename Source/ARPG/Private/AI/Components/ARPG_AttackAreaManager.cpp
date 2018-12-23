// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_AttackAreaManager.h"
#include "CharacterBase.h"

UWorld* UARPG_AttackExecuterBase::GetWorld() const
{
	return Attacker ? Attacker->GetWorld() : nullptr;
}

bool UAC_CoolDown::CanAttack(ACharacterBase* Attacker) const
{
	return NextExecuteSecond < Attacker->GetWorld()->GetTimeSeconds();
}

void UAC_CoolDown::WhenExecuteAttack(ACharacterBase* Attacker)
{
	NextExecuteSecond = Attacker->GetWorld()->GetTimeSeconds() + CoolDownTime + FMath::FRand() * RandomRange - RandomRange / 2.f;
}

FVector UARPG_AttackArea_Sphere::GetAttackMoveLocation(ACharacterBase* Attacker, AActor* AttackTarget) const
{
	FVector TargetLocation = AttackTarget->GetActorLocation();
	return TargetLocation + (Attacker->GetActorLocation() - TargetLocation).GetSafeNormal2D() * GetWorldRadius(Attacker->GetActorScale()) * 0.5f;
}

bool UARPG_AttackArea_Sphere::IsInArea(ACharacterBase* Attacker, AActor* AttackTarget) const
{
	FTransform WorldTransform = Attacker->GetTransform();
	float Distance = (AttackTarget->GetActorLocation() - WorldTransform.TransformPosition(Origin)).Size();
	return Distance < GetWorldRadius(Attacker->GetActorScale());
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
	FVector RelativeLocation = WorldTransform.InverseTransformPosition(AttackTarget->GetActorLocation()) - Origin;
	return FBox(-Extent / 2.f, Extent / 2.f).IsInside(RelativeLocation);
}

void UAE_NormalMontage::InvokeAttack(AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	Attacker->PlayMontageWithBlendingOutDelegate(Montage, FOnMontageBlendingOutStarted::CreateUObject(this, &UAE_NormalMontage::WhenMontageBlendOutStart, OnAttackFinished));
}

void UAE_NormalMontage::WhenMontageBlendOutStart(UAnimMontage* CurMontage, bool bInterrupted, FBP_OnAttackFinished OnAttackFinished)
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
			if (AttackAreaParam.AttackExecuter)
			{
				AttackAreaParam.AttackExecuter->Attacker = Character;
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

bool UARPG_AttackAreaManager::IsAllowedAttack_Implementation(class AActor* AttackTarget) const
{
	for (int32 i = 0; i < AttackConfigs.Num(); ++i)
	{
		const FAttackAreaParam& AttackAreaParam = AttackConfigs[i];
		if (AttackAreaParam.CanAttack(Attacker, AttackTarget))
		{
			ActiveAttackConfigIndex = i;
			return true;
		}
	}
	return false;
}

void UARPG_AttackAreaManager::InvokeAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackFinished& OnAttackFinished)
{
	if (ActiveAttackConfigIndex != INDEX_NONE)
	{
		const FAttackAreaParam& AttackAreaParam = AttackConfigs[ActiveAttackConfigIndex];
		if (AttackAreaParam.Condition)
		{
			AttackAreaParam.Condition->WhenExecuteAttack(Attacker);
		}
		AttackAreaParam.AttackExecuter->InvokeAttack(AttackTarget, FARPG_OnAttackFinished::CreateUObject(this, &UARPG_AttackAreaManager::WhenAttackFinished, OnAttackFinished));
	}
	else
	{
		OnAttackFinished.ExecuteIfBound(false);
	}
}

void UARPG_AttackAreaManager::AbortAttack_Implementation(class AActor* AttackTarget, const FBP_OnAttackAborted& OnAttackAborted)
{
	if (UARPG_AttackExecuterBase* AttackParam = GetActiveAttackParam())
	{
		AttackParam->AbortAttack(AttackTarget, OnAttackAborted);
	}
	else
	{
		OnAttackAborted.ExecuteIfBound();
	}
}

void UARPG_AttackAreaManager::AttackingTick_Implementation(class AActor* AttackTarget, float DeltaSecond)
{
	if (UARPG_AttackExecuterBase* AttackParam = GetActiveAttackParam())
	{
		AttackParam->AttackingTick(AttackTarget, DeltaSecond);
	}
}

void UARPG_AttackAreaManager::WhenAttackFinished(bool Succeed, FBP_OnAttackFinished OnAttackFinished)
{
	ActiveAttackConfigIndex = INDEX_NONE;
	OnAttackFinished.ExecuteIfBound(Succeed);
}
