// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include "ARPG_ProjectileMovementComponent.generated.h"

/**
 * 
 */
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
