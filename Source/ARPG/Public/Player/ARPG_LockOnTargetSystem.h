// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARPG_LockOnTargetSystem.generated.h"

USTRUCT(BlueprintType, meta = (BlueprintInternalUseOnly = "true"))
struct ARPG_API FARPG_LockOnTargetSystem
{
	GENERATED_BODY()
public:
	FARPG_LockOnTargetSystem();

	UPROPERTY(BlueprintReadOnly, Category = "锁定系统")
	TSoftObjectPtr<AActor> LockedTarget;
	
	UPROPERTY(BlueprintReadOnly, Category = "锁定系统")
	FName LockedSocketName;

	UPROPERTY(BlueprintReadWrite, Category = "锁定系统")
	uint8 bEnableLockOnSystem : 1;

	void LockOnTargetTick(class AController* Controller, float DeltaTime);

	bool CanLockedOn(class AController* Controller, AActor* Target, const FName& SocketName);

	void SetLockedTarget(AActor* Target, const FName& SocketName);

	void ClearLockedTarget();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "锁定系统")
	float MaxTraceDistance = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "锁定系统")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "锁定系统")
	TEnumAsByte<ETraceTypeQuery> VisibilityChannel;

	TArray<AActor*> GetIgnoreActors(class AController* Controller) const;

	bool TraceCanLockedActor(class AController* Controller, const FVector& Start, const FVector& End, float Radius = 500.f);

	bool FilterResultToLock(TArray<FHitResult> HitResults, class AController* Controller);

	void ToggleLockedTarget(class AController* Controller);
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UARPG_LockOnTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_API IARPG_LockOnTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "锁定系统")
	FVector GetTargetLocation(const FName& CurLockSocketName) const;
	virtual FVector GetTargetLocation_Implementation(const FName& CurLockSocketName) const { return FVector::ZeroVector; }

	UFUNCTION(BlueprintNativeEvent, Category = "锁定系统")
	bool CanLockedOnTarget(AController* Invoker, const FName& InvokeLockedSocketName) const;
	virtual bool CanLockedOnTarget_Implementation(AController* Invoker, const FName& InvokeLockedSocketName) const { return true; }

	UFUNCTION(BlueprintNativeEvent, Category = "锁定系统")
	bool CanLockingOnTarget(AController* Invoker, const FName& CurLockSocketName, bool& TryLockAgain) const;
	virtual bool CanLockingOnTarget_Implementation(AController* Invoker, const FName& CurLockSocketName, bool& TryLockAgain) const { return true; }
};
