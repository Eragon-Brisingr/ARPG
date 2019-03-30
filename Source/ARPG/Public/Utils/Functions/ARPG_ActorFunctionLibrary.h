// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XD_ActorFunctionLibrary.h"
#include "ARPG_ActorFunctionLibrary.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ARPG_API UARPG_ActorFunctionLibrary : public UXD_ActorFunctionLibrary
{
	GENERATED_BODY()
public:

};

DECLARE_DELEGATE_OneParam(FOnActorMoveFinished, bool /*bIsAborted*/);

UCLASS()
class ARPG_API UARPG_ActorMoveUtils : public UXD_ActorFunctionLibrary
{
	GENERATED_BODY()
public:
	struct FActorMoveData
	{
		FActorMoveData() = default;

		FActorMoveData(const FDelegateHandle& TickerHandle, const FOnActorMoveFinished& OnActorMoveFinished)
			:TickerHandle(TickerHandle), OnActorMoveFinished(OnActorMoveFinished)
		{}

		FDelegateHandle TickerHandle;
		FOnActorMoveFinished OnActorMoveFinished;
	};

	static TMap<TWeakObjectPtr<USceneComponent>, FActorMoveData> MovingComponentMap;

	static void MoveComponentTo(USceneComponent* Component, const FVector& TargetRelativeLocation, const FRotator& TargetRelativeRotation, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);
	static void MoveComponentToLocation(USceneComponent* Component, const FVector& TargetRelativeLocation, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);
	static void MoveComponentToRotation(USceneComponent* Component, const FRotator& TargetRelativeRotation, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);

	static void PushComponentTo(USceneComponent* Component, const FVector& Distance, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = true);

	static void MoveActorTo(AActor* Actor, const FVector& Location, const FRotator& Rotation, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);
	static void MoveActorToLocation(AActor* Actor, const FVector& Location, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);
	static void MoveActorToRotation(AActor* Actor, const FRotator& Rotation, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);

	static void PushActorTo(AActor* Actor, const FVector& Distance, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = true);

	static void MoveCharacterToFitGround(ACharacterBase* Character, const FVector& Location, const FRotator& Rotator, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);

	static void MoveCharacterToLocationFitGround(ACharacterBase* Character, const FVector& Location, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);

	static void MoveCharacterToRotationFitGround(ACharacterBase* Character, const FRotator& Rotator, const FOnActorMoveFinished& OnMoveFinished = {}, float OverTime = 0.2f, bool Sweep = false);

	static void StopComponentMove(USceneComponent* Component);

	static void StopActorMove(AActor* Actor);
};
