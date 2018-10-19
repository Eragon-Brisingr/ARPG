﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "ARPG_ArrowBase.generated.h"

UCLASS()
class ARPG_API UARPG_ProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
public:
	UARPG_ProjectileMovementComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void Activate(bool bReset = false) override;

	virtual void Deactivate() override;
public:
	UPROPERTY(EditAnywhere, Category = "检测", meta = (MakeEditWidget = "true"))
	FVector TraceOriginOffet;

	FVector PreLocation;

	UPROPERTY(EditAnywhere, Category = "检测")
	float Radius = 5.f;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	TWeakObjectPtr<class UPrimitiveComponent> TracedComponent;

	UPROPERTY(EditAnywhere, Category = "检测")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, Category = "检测")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	DECLARE_DELEGATE_FourParams(FOnTraceActorNative, USceneComponent*, AActor*, UPrimitiveComponent*, const FHitResult&);
	FOnTraceActorNative OnTraceActorNative;
};

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
public:
	void WhenHitCharacter(USceneComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit);
	
	UFUNCTION()
	void WhenArrowHitEnvironment(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PostArrowHitOther(UARPG_ProjectileMovementComponent* ProjectileMovementComponent);

	UFUNCTION(Unreliable, NetMulticast, WithValidation)
	void Launch(float ForceSize);
	void Launch_Implementation(float ForceSize);
	bool Launch_Validate(float ForceSize) { return true; }

	void ClientArrowStop();

	virtual void OnRep_AttachmentReplication() override;

};
