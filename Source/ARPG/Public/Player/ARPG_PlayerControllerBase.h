// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARPG_LockOnTargetSystem.h"
#include "ARPG_PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API AARPG_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, Category = "锁定系统", meta = (ShowOnlyInnerProperties))
	FARPG_LockOnTargetSystem LockOnTargetSystem;

	AActor* GetLockedTarget() const;

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	void EnableLockOnTargetSystem(bool Enable) { LockOnTargetSystem.bEnableLockOnSystem = Enable; }

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	bool SetLockedTarget(AActor* Target, const FName& SocketName);

	UFUNCTION(Server, WithValidation, Reliable)
	void SetLockedTarget_ToServer(AActor* Target, const FName& SocketName);
	void SetLockedTarget_ToServer_Implementation(AActor* Target, const FName& SocketName);
	bool SetLockedTarget_ToServer_Validate(AActor* Target, const FName& SocketName) { return true; }

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	void ClearLockedTarget();

	UFUNCTION(Server, WithValidation, Reliable)
	void ClearLockedTarget_ToServer();
	void ClearLockedTarget_ToServer_Implementation();
	bool ClearLockedTarget_ToServer_Validate() { return true; }

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	void ToggleLockedTarget() { LockOnTargetSystem.ToggleLockedTarget(this); }

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	bool InvokeSwitchLockedTarget(bool Left);
};
