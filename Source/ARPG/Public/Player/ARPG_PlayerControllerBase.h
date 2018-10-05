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

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	bool SetLockedTarget(AActor* Target, const FName& SocketName);

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	void ClearLockedTarget() { LockOnTargetSystem.ClearLockedTarget(); }

	UFUNCTION(BlueprintCallable, Category = "锁定系统")
	void ToggleLockedTarget() { LockOnTargetSystem.ToggleLockedTarget(this); }
};
