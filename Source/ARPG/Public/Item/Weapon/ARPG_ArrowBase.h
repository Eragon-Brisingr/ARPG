// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include "ARPG_ArrowBase.generated.h"


/**
 * 
 */
UCLASS(Abstract)
class ARPG_API AARPG_ArrowBase : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;

	virtual void UseItemImpl_Implementation(class UARPG_ItemCoreBase* ItemCore, class ACharacterBase* ItemOwner, EUseItemInput UseItemInput) const override;

	virtual void WhenRemoveFromInventory_Implementation(class AActor* ItemOwner, class UXD_ItemCoreBase* ItemCore, int32 RemoveNumber, int32 ExistNumber) const override;
public:
	void WhenHitCharacter(USceneComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit);
	
	UFUNCTION()
	void WhenArrowHitEnvironment(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PostArrowHitOther(class UARPG_ProjectileMovementComponent* ProjectileMovementComponent);

	UFUNCTION(Unreliable, NetMulticast, WithValidation)
	void Launch(float ForceSize);
	void Launch_Implementation(float ForceSize);
	bool Launch_Validate(float ForceSize) { return true; }

	void ClientArrowStop();

	virtual void OnRep_AttachmentReplication() override;

};
