// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ActorFunctionLibrary.h"
#include "ARPG_ActorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_ActorFunctionLibrary : public UXD_ActorFunctionLibrary
{
	GENERATED_BODY()
public:
	static TMap<TWeakObjectPtr<USceneComponent>, FDelegateHandle> MovingComponentMap;

	UFUNCTION(BlueprintCallable, Category = "游戏|移动")
	static void MoveComponentTo(USceneComponent* Component, const FVector& TargetRelativeLocation, const FRotator& TargetRelativeRotation, float OverTime = 0.2f, bool Sweep = true);
	
	UFUNCTION(BlueprintCallable, Category = "游戏|移动")
	static void PushComponentTo(USceneComponent* Component, const FVector& Distance, float OverTime = 0.2f, bool Sweep = true);

	UFUNCTION(BlueprintCallable, Category = "游戏|移动")
	static void MoveActorTo(AActor* Actor, const FVector& Location, const FRotator& Rotator, float OverTime = 0.2f, bool Sweep = true);

	UFUNCTION(BlueprintCallable, Category = "游戏|移动")
	static void PushActorTo(AActor* Actor, const FVector& Distance, float OverTime = 0.2f, bool Sweep = true);
};
