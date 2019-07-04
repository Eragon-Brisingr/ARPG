// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include "CharacterDamageType.h"
#include "ARPG_ArrowBase.generated.h"

class UARPG_ArrowCoreBase;


/**
 * 
 */
UCLASS(Abstract, meta = (DisplayName = "箭"))
class ARPG_API AARPG_ArrowBase : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;

	virtual void UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const override;

	virtual void WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const override;
public:
	void WhenHitCharacter(USceneComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit, FApplyPointDamageParameter ApplyPointDamageParameter);

	float ApplyDamamgeToCharacter(ACharacterBase* Character, const FHitResult& Hit, const FApplyPointDamageParameter& ApplyPointDamageParameter);

	UFUNCTION()
	void WhenArrowHitEnvironment(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float GetArrowDamage();

	void PostArrowHitOther(class UARPG_ProjectileMovementComponent* ProjectileMovementComponent);

	void Launch(float ForceSize, const FApplyPointDamageParameter& ApplyPointDamageParameter);

	UFUNCTION(Unreliable, NetMulticast, WithValidation)
	void Launch_Multicast(float ForceSize);
	void Launch_Multicast_Implementation(float ForceSize);
	bool Launch_Multicast_Validate(float ForceSize) { return true; }

	void ClientArrowStop();

	virtual void OnRep_AttachmentReplication() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "武器", meta = (DisplayName = "箭袋"))
	UStaticMesh* Dorlach;

	void WhenUse(class ACharacterBase* ItemOwner) override;

	void ToEquippedDorlachMode();

public:
	const UARPG_ArrowCoreBase* GetItemCore() const;
	UARPG_ArrowCoreBase* GetItemCore_Careful() const;
};
