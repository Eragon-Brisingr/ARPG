// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPG_ReceiveDamageActionBase.h"
#include "CharacterBase.h"
#include "ReceiveDamageActionBase.h"

void UARPG_ReceiveDamageActionSimpleBase::PlayHitStunMontage(ACharacterBase* Character, float BaseDamage, float HitStunOverflowValue, const FVector& HitFromDirection, const FHitResult& HitResult, class ACharacterBase* InstigatedBy, AActor* DamageCauser) const
{
	Character->PlayMontage(HitStunMontage, FARPG_MontagePlayConfig());
}

void UARPG_ReceiveDamageActionSimpleBase::PlayNormalAdditiveDamageMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, class ACharacterBase* InstigatedBy, AActor* DamageCauser) const
{
	Character->PlayMontage(DamageAdditiveMontage, FARPG_MontagePlayConfig());
}

void UARPG_ReceiveDamageActionSimpleBase::PlayNormalPointDamageDeadMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, class ACharacterBase* InstigatedBy, AActor* DamageCauser) const
{
	Character->PlayMontage(PointDamageDeadMontage, FARPG_MontagePlayConfig());
}

bool UARPG_ReceiveDamageActionSimpleBase::PlayReceiveDamageSpecialAction(ACharacterBase* Character, TSubclassOf<UReceiveDamageActionBase> ReceiveDamageAction, const FVector& HitFromDirection, const FHitResult& HitResult, class ACharacterBase* InstigatedBy, AActor* DamageCauser) const
{
	return ReceiveDamageAction.GetDefaultObject()->PlayReceiveDamageAction(HitFromDirection, Character, HitResult, InstigatedBy, DamageCauser);
}
