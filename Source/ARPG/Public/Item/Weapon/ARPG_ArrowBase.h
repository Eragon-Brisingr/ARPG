// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPG_WeaponBase.h"
#include <Tickable.h>
#include <Kismet/KismetSystemLibrary.h>
#include "ARPG_ArrowBase.generated.h"

UCLASS(editinlinenew)
class ARPG_API UProjectileTracer : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UProjectileTracer();

	// Inherited via FTickableGameObject
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UProjectileTracer, STATGROUP_Tickables); }

	virtual bool IsTickable() const { return bTraceable; }

public:
	UPROPERTY(EditAnywhere, Category = "检测")
	uint8 bTraceable : 1;

	UPROPERTY(EditAnywhere, Category = "检测")
	uint8 bRotationUseVelocity : 1;

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

	UFUNCTION(BlueprintCallable, Category = "检测")
	void SetTraceEnable(bool Enable);

	UFUNCTION(BlueprintCallable, Category = "检测")
	void SetTargetComponent(class UPrimitiveComponent* TargetComponent);

	DECLARE_DELEGATE_FourParams(FOnTraceActorNative, UPrimitiveComponent*, AActor*, UPrimitiveComponent*, const FHitResult&);
	FOnTraceActorNative OnTraceActorNative;
};

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_ArrowBase : public AARPG_WeaponBase
{
	GENERATED_BODY()
public:
	AARPG_ArrowBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "检测", Instanced)
	UProjectileTracer* ProjectileTracer;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void WhenHitCharacter(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, const FHitResult& Hit);

	void PostArrowHitOther();

	UFUNCTION(BlueprintCallable, Category = "角色|行为")
	void Release(float ForceSize);
};
