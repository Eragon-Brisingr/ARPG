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
	void WhenUse(class ACharacterBase* ItemOwner) override;
public:
	const UARPG_ArrowCoreBase* GetItemCoreConst() const;
	UARPG_ArrowCoreBase* GetItemCore() const;
};

UCLASS()
class ARPG_API AARPG_Arrow_StaticMesh : public AARPG_ArrowBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Arrow_StaticMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

	void InitItemMesh() override { InitStaticMeshComponent(StaticMeshComponent); }
};

UCLASS()
class ARPG_API AARPG_Arrow_SkeletalMesh : public AARPG_ArrowBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AARPG_Arrow_SkeletalMesh(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	void InitItemMesh() override { InitSkeletalMeshComponent(SkeletalMeshComponent); }
};
