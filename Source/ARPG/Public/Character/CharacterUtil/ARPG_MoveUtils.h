// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ARPG_MoveUtils.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnARPG_MoveFinished, const FPathFollowingResult& /*Result*/);

UCLASS()
class ARPG_API UARPG_MoveUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	friend class UARPG_CharacterMove_AsyncAction;
private:
	static FPathFollowingRequestResult ARPG_MoveToActorImpl(class ACharacterBase* Character, AActor* Goal, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bCanStrafe, TSubclassOf<class UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths);

	static FPathFollowingRequestResult ARPG_MoveToLocationImpl(class ACharacterBase* Character, const FVector& Dest, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths);

	static FPathFollowingRequestResult MoveTo(class AController* Controller, class UPathFollowingComponent* PathFollowingComponent, const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr);

	static bool BuildPathfindingQuery(const class AController* Controller, class UPathFollowingComponent* PathFollowingComponent, const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query);

	static void FindPathForMoveRequest(const class AController* Controller, const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath);

public:
	static class UPathFollowingComponent* GetPathFollowingComponent(const class AController* Controller);

	static void ARPG_MoveToActor(class ACharacterBase* Character, AActor* Goal, const FOnARPG_MoveFinished& OnARPG_MoveFinished, float AcceptanceRadius = 5.f, bool bStopOnOverlap = true, bool bUsePathfinding = true, bool bCanStrafe = true, TSubclassOf<class UNavigationQueryFilter> FilterClass = nullptr, bool bAllowPartialPaths = true);

	static void ARPG_MoveToLocation(class ACharacterBase* Character, const FVector& Dest, const FOnARPG_MoveFinished& OnARPG_MoveFinished, float AcceptanceRadius = 5.f, bool bStopOnOverlap = true, bool bUsePathfinding = true, bool bProjectDestinationToNavigation = false, bool bCanStrafe = true, TSubclassOf<UNavigationQueryFilter> FilterClass = nullptr, bool bAllowPartialPaths = true);

	static void SettingRequest(FPathFollowingRequestResult &ResultData, class ACharacterBase* Character, const FOnARPG_MoveFinished &OnARPG_MoveFinished);

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, FOnARPG_MoveFinished OnARPG_MoveFinish, UPathFollowingComponent* PathFollowingComponent, FAIRequestID InvokeRequestID);
}; 

UCLASS()
class ARPG_API UARPG_CharacterMove_AsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "角色|移动", meta = (BlueprintInternalUseOnly = "true", DisplayName = "ARPG MoveToActor", AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPaths"))
	static class UARPG_CharacterMove_AsyncAction* BP_ARPG_MoveToActor(class ACharacterBase* Character, AActor* Goal, float AcceptanceRadius = 5.f, bool bStopOnOverlap = true, bool bUsePathfinding = true, bool bCanStrafe = true, TSubclassOf<class UNavigationQueryFilter> FilterClass = nullptr, bool bAllowPartialPaths = true);

	UFUNCTION(BlueprintCallable, Category = "角色|移动", meta = (BlueprintInternalUseOnly = "true", DisplayName = "ARPG MoveToLocation", AdvancedDisplay = "bStopOnOverlap,bCanStrafe,bAllowPartialPaths,bProjectDestinationToNavigation"))
	static class UARPG_CharacterMove_AsyncAction* BP_ARPG_MoveToLocation(class ACharacterBase* Character, const FVector& Dest, float AcceptanceRadius = 5.f, bool bStopOnOverlap = true, bool bUsePathfinding = true, bool bProjectDestinationToNavigation = false, bool bCanStrafe = true, TSubclassOf<UNavigationQueryFilter> FilterClass = nullptr, bool bAllowPartialPaths = true);
public:
	UPROPERTY()
	class ACharacterBase* Character;

	void OnMoveCompletedCheckedId(FAIRequestID RequestID, const FPathFollowingResult& Result, UPathFollowingComponent* PathFollowingComponent, FAIRequestID InvokeRequestID);

	void OnMoveCompleted(FPathFollowingResult Result);

	static UARPG_CharacterMove_AsyncAction* SettingRequest(class ACharacterBase* Character, FPathFollowingRequestResult &ResultData);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMoveFinishedSignature, TEnumAsByte<EPathFollowingResult::Type>, MoveResult, class ACharacterBase*, Mover);
	UPROPERTY(BlueprintAssignable)
	FOnMoveFinishedSignature OnMoveFinished;
};
