// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ARPG_ReceiveDamageActionBase.generated.h"

class ACharacterBase;
class UReceiveDamageActionBase;
class UAnimMontage;

/**
 * 
 */
UCLASS(abstract, Blueprintable, const)
class ARPG_API UARPG_ReceiveDamageActionBase : public UDataAsset
{
	GENERATED_BODY()

	// 受击
public:
	//硬直动画，会打断攻击行为
	virtual void PlayHitStunMontage(ACharacterBase* Character, float BaseDamage, float HitStunOverflowValue, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const
	{
		ReceivePlayHitStunMontage(Character, BaseDamage, HitStunOverflowValue, HitFromDirection, HitResult, InstigatedBy, DamageCauser);
	}
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "行为|受击", meta = (DisplayName = "PlayHitStunMontage"))
	void ReceivePlayHitStunMontage(ACharacterBase* Character, float BaseDamage, float HitStunOverflowValue, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const;

	virtual void PlayNormalAdditiveDamageMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const
	{
		ReceivePlayNormalAdditiveDamageMontage(Character, BaseDamage, HitFromDirection, HitResult, InstigatedBy, DamageCauser);
	}
	//普通状态下的受击动画，不会打断攻击行为
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "行为|受击", meta = (DisplayName = "PlayNormalDamageMontage"))
	void ReceivePlayNormalAdditiveDamageMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const;

	//特殊受击行为动画，返回值代表是否播放了受击动作
	virtual bool PlayReceiveDamageSpecialAction(ACharacterBase* Character, TSubclassOf<UReceiveDamageActionBase> ReceiveDamageAction, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const
	{
		return ReceivePlayReceiveDamageSpecialAction(Character, ReceiveDamageAction, HitFromDirection, HitResult, InstigatedBy, DamageCauser);
	}
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "行为|受击", meta = (DisplayName = "PlayReceiveDamageAction"))
	bool ReceivePlayReceiveDamageSpecialAction(ACharacterBase* Character, TSubclassOf<UReceiveDamageActionBase> ReceiveDamageAction, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const;

	// 死亡
public:
	virtual void PlayNormalPointDamageDeadMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const
	{
		ReceivePlayNormalPointDamageDeadMontage(Character, BaseDamage, HitFromDirection, HitResult, InstigatedBy, DamageCauser);
	}
	// 受击死亡动画
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "行为|死亡", meta = (DisplayName = "PlayNormalPointDamageDeadMontage"))
	void ReceivePlayNormalPointDamageDeadMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const;

	// 普通死亡动画
	virtual void PlayNormalDeadMontage(ACharacterBase* Character) const { ReceivePlayNormalDeadMontage(Character); }
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, Category = "行为|死亡", meta = (DisplayName = "PlayNormalDeadMontage"))
	void ReceivePlayNormalDeadMontage(ACharacterBase* Character) const;
};

UCLASS(meta = (DisplayName = "角色受击_简单行为"))
class ARPG_API UARPG_ReceiveDamageActionSimpleBase : public UARPG_ReceiveDamageActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "受击动画配置")
	UAnimMontage* HitStunMontage;
	UPROPERTY(EditAnywhere, Category = "受击动画配置")
	UAnimMontage* DamageAdditiveMontage;
	UPROPERTY(EditAnywhere, Category = "受击动画配置")
	UAnimMontage* PointDamageDeadMontage;
	UPROPERTY(EditAnywhere, Category = "受击动画配置")
	UAnimMontage* NormalDeadMontage;

	void PlayHitStunMontage(ACharacterBase* Character, float BaseDamage, float HitStunOverflowValue, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const override;
	void PlayNormalAdditiveDamageMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const override;
	bool PlayReceiveDamageSpecialAction(ACharacterBase* Character, TSubclassOf<UReceiveDamageActionBase> ReceiveDamageAction, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const override;
	void PlayNormalPointDamageDeadMontage(ACharacterBase* Character, float BaseDamage, const FVector& HitFromDirection, const FHitResult& HitResult, const UObject* InstigatedBy, const UObject* DamageCauser) const override;
	void PlayNormalDeadMontage(ACharacterBase* Character) const override;
};
