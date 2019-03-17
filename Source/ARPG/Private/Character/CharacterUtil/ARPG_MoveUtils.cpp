// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_MoveUtils.h"
#include "NavigationSystem.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "NavigationQueryFilter.h"
#include "CharacterBase.h"
#include "TimerManager.h"
#include "ARPG_PlayerControllerBase.h"

FPathFollowingRequestResult UARPG_MoveUtils::ARPG_MoveToActorImpl(class ACharacterBase* Character, AActor* Goal, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bCanStrafe, TSubclassOf<class UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	Character->WhenReceivedMoveRequest();

	AController* Controller = Character->GetController();
	UPathFollowingComponent* PathFollowingComponent = GetPathFollowingComponent(Controller);

	if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowingComponent->AbortMove(*Controller, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}

	FAIMoveRequest MoveReq(Goal);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetNavigationFilter(FilterClass ? FilterClass : Character->NavigationQueryFilter);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);

	return MoveTo(Controller, PathFollowingComponent, MoveReq);
}

FPathFollowingRequestResult UARPG_MoveUtils::ARPG_MoveToLocationImpl(class ACharacterBase* Character, const FVector& Dest, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	Character->WhenReceivedMoveRequest();

	AController* Controller = Character->GetController();
	UPathFollowingComponent* PathFollowingComponent = GetPathFollowingComponent(Controller);

	if (PathFollowingComponent && PathFollowingComponent->GetStatus() != EPathFollowingStatus::Idle)
	{
		PathFollowingComponent->AbortMove(*Controller, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::CurrentRequest, EPathFollowingVelocityMode::Keep);
	}

	FAIMoveRequest MoveReq(Dest);
	MoveReq.SetUsePathfinding(bUsePathfinding);
	MoveReq.SetAllowPartialPath(bAllowPartialPaths);
	MoveReq.SetProjectGoalLocation(bProjectDestinationToNavigation);
	MoveReq.SetNavigationFilter(FilterClass ? FilterClass : Character->NavigationQueryFilter);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
	MoveReq.SetCanStrafe(bCanStrafe);

	return MoveTo(Controller, PathFollowingComponent, MoveReq);
}

FPathFollowingRequestResult UARPG_MoveUtils::MoveTo(class AController* Controller, class UPathFollowingComponent* PathFollowingComponent, const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath)
{
	if (AARPG_PlayerControllerBase* PlayerController = Cast<AARPG_PlayerControllerBase>(Controller))
	{
		PlayerController->bIsInPathFollowing = true;
	}

	class APawn* Pawn = Controller->GetPawn();

	// both MoveToActor and MoveToLocation can be called from blueprints/script and should keep only single movement request at the same time.
	// this function is entry point of all movement mechanics - do NOT abort in here, since movement may be handled by AITasks, which support stacking 

	FPathFollowingRequestResult ResultData;
	ResultData.Code = EPathFollowingRequestResult::Failed;

	if (MoveRequest.IsValid() == false)
	{
		return ResultData;
	}

	bool bCanRequestMove = true;
	bool bAlreadyAtGoal = false;

	if (!MoveRequest.IsMoveToActorRequest())
	{
		if (MoveRequest.GetGoalLocation().ContainsNaN() || FAISystem::IsValidLocation(MoveRequest.GetGoalLocation()) == false)
		{
			bCanRequestMove = false;
		}

		// fail if projection to navigation is required but it failed
		if (bCanRequestMove && MoveRequest.IsProjectingGoal())
		{
			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Controller->GetWorld());
			const FNavAgentProperties& AgentProps = Pawn->GetNavAgentPropertiesRef();
			FNavLocation ProjectedLocation;

			if (NavSys && !NavSys->ProjectPointToNavigation(MoveRequest.GetGoalLocation(), ProjectedLocation, INVALID_NAVEXTENT, &AgentProps))
			{
				bCanRequestMove = false;
			}

			MoveRequest.UpdateGoalLocation(ProjectedLocation.Location);
		}

		bAlreadyAtGoal = bCanRequestMove && PathFollowingComponent->HasReached(MoveRequest);
	}
	else
	{
		bAlreadyAtGoal = bCanRequestMove && PathFollowingComponent->HasReached(MoveRequest);
	}

	if (bAlreadyAtGoal)
	{
		ResultData.MoveId = PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
		ResultData.Code = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else if (bCanRequestMove)
	{
		FPathFindingQuery PFQuery;

		const bool bValidQuery = BuildPathfindingQuery(Controller, PathFollowingComponent, MoveRequest, PFQuery);
		if (bValidQuery)
		{
			FNavPathSharedPtr Path;
			FindPathForMoveRequest(Controller, MoveRequest, PFQuery, Path);

			const FAIRequestID RequestID = Path.IsValid() ? PathFollowingComponent->RequestMove(MoveRequest, Path) : FAIRequestID::InvalidRequest;
			if (RequestID.IsValid())
			{
				if (AAIController* AIController = Cast<AAIController>(Controller))
				{
					AIController->bAllowStrafe = MoveRequest.CanStrafe();
				}
				ResultData.MoveId = RequestID;
				ResultData.Code = EPathFollowingRequestResult::RequestSuccessful;

				if (OutPath)
				{
					*OutPath = Path;
				}
			}
		}
	}

	if (ResultData.Code == EPathFollowingRequestResult::Failed)
	{
		ResultData.MoveId = PathFollowingComponent->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
	}

	return ResultData;
}

bool UARPG_MoveUtils::BuildPathfindingQuery(const class AController* Controller, class UPathFollowingComponent* PathFollowingComponent, const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query)
{
	class APawn* Pawn = Controller->GetPawn();

	bool bResult = false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Pawn->GetWorld());
	const ANavigationData* NavData = (NavSys == nullptr) ? nullptr :
		MoveRequest.IsUsingPathfinding() ? NavSys->GetNavDataForProps(Pawn->GetNavAgentPropertiesRef()) :
		NavSys->GetAbstractNavData();

	if (NavData)
	{
		FVector GoalLocation = MoveRequest.GetGoalLocation();
		if (MoveRequest.IsMoveToActorRequest())
		{
			const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
			if (NavGoal)
			{
				const FVector Offset = NavGoal->GetMoveGoalOffset(Controller);
				GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(), NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
			}
			else
			{
				GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
			}
		}

		FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, Controller, MoveRequest.GetNavigationFilter());
		Query = FPathFindingQuery(*Controller, *NavData, Pawn->GetNavAgentLocation(), GoalLocation, NavFilter);
		Query.SetAllowPartialPaths(MoveRequest.IsUsingPartialPaths());

		if (PathFollowingComponent)
		{
			PathFollowingComponent->OnPathfindingQuery(Query);
		}

		bResult = true;
	}

	return bResult;
}

void UARPG_MoveUtils::FindPathForMoveRequest(const class AController* Controller, const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Controller->GetWorld());
	if (NavSys)
	{
		FPathFindingResult PathResult = NavSys->FindPathSync(Query);
		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				if (MoveRequest.IsMoveToActorRequest())
				{
					PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
				}

				PathResult.Path->EnableRecalculationOnInvalidation(true);
				OutPath = PathResult.Path;
			}
		}
	}
}

class UPathFollowingComponent* UARPG_MoveUtils::GetPathFollowingComponent(const class AController* Controller)
{
	if (const AAIController* AIController = Cast<const AAIController>(Controller))
	{
		return AIController->GetPathFollowingComponent();
	}
	else if (const AARPG_PlayerControllerBase* PlayerController = Cast<const AARPG_PlayerControllerBase>(Controller))
	{
		return (UPathFollowingComponent*)PlayerController->PathFollowingComponent;
	}
	else
	{
		return Controller->FindComponentByClass<UPathFollowingComponent>();
	}
}

void UARPG_MoveUtils::ARPG_MoveToActor(class ACharacterBase* Character, AActor* Goal, const FOnARPG_MoveFinished& OnARPG_MoveFinished, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bCanStrafe, TSubclassOf<class UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	FPathFollowingRequestResult ResultData = ARPG_MoveToActorImpl(Character, Goal, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bCanStrafe, FilterClass, bAllowPartialPaths);
	SettingRequest(ResultData, Character, OnARPG_MoveFinished);
}

void UARPG_MoveUtils::ARPG_MoveToLocation(class ACharacterBase* Character, const FVector& Dest, const FOnARPG_MoveFinished& OnARPG_MoveFinished, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	FPathFollowingRequestResult ResultData = ARPG_MoveToLocationImpl(Character, Dest, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bProjectDestinationToNavigation, bCanStrafe, FilterClass, bAllowPartialPaths);
	SettingRequest(ResultData, Character, OnARPG_MoveFinished);
}

void UARPG_MoveUtils::SettingRequest(FPathFollowingRequestResult &ResultData, class ACharacterBase* Character, const FOnARPG_MoveFinished &OnARPG_MoveFinished)
{
	switch (ResultData.Code)
	{
	case EPathFollowingRequestResult::RequestSuccessful:
	{
		UPathFollowingComponent* PathFollowingComponent = GetPathFollowingComponent(Character->GetController());
		PathFollowingComponent->OnRequestFinished.AddUObject(GetDefault<UARPG_MoveUtils>(), &UARPG_MoveUtils::OnMoveCompleted, OnARPG_MoveFinished, PathFollowingComponent, ResultData.MoveId);
	}
	break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		OnARPG_MoveFinished.ExecuteIfBound(FPathFollowingResult(EPathFollowingResult::Success, FPathFollowingResultFlags::None));
	}
	break;
	case EPathFollowingRequestResult::Failed:
	{
		OnARPG_MoveFinished.ExecuteIfBound(FPathFollowingResult(EPathFollowingResult::Invalid, FPathFollowingResultFlags::InvalidPath));
	}
	break;
	}
}

void UARPG_MoveUtils::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, FOnARPG_MoveFinished OnARPG_MoveFinish, UPathFollowingComponent* PathFollowingComponent, FAIRequestID InvokeRequestID)
{
	if (RequestID.IsEquivalent(InvokeRequestID))
	{
		if (AARPG_PlayerControllerBase* PlayerController = Cast<AARPG_PlayerControllerBase>(PathFollowingComponent->GetOwner()))
		{
			PlayerController->bIsInPathFollowing = false;
		}
		PathFollowingComponent->OnRequestFinished.RemoveAll(this);
		OnARPG_MoveFinish.ExecuteIfBound(Result);
	}
}

UARPG_CharacterMove_AsyncAction* UARPG_CharacterMove_AsyncAction::BP_ARPG_MoveToActor(class ACharacterBase* Character, AActor* Goal, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bCanStrafe, TSubclassOf<class UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	if (Character && Character->GetController())
	{
		FPathFollowingRequestResult ResultData = UARPG_MoveUtils::ARPG_MoveToActorImpl(Character, Goal, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bCanStrafe, FilterClass, bAllowPartialPaths);
		return UARPG_CharacterMove_AsyncAction::SettingRequest(Character, ResultData);
	}
	return nullptr;
}

UARPG_CharacterMove_AsyncAction* UARPG_CharacterMove_AsyncAction::BP_ARPG_MoveToLocation(class ACharacterBase* Character, const FVector& Dest, float AcceptanceRadius, bool bStopOnOverlap, bool bUsePathfinding, bool bProjectDestinationToNavigation, bool bCanStrafe, TSubclassOf<UNavigationQueryFilter> FilterClass, bool bAllowPartialPaths)
{
	if (Character && Character->GetController())
	{
		FPathFollowingRequestResult ResultData = UARPG_MoveUtils::ARPG_MoveToLocationImpl(Character, Dest, AcceptanceRadius, bStopOnOverlap, bUsePathfinding, bProjectDestinationToNavigation, bCanStrafe, FilterClass, bAllowPartialPaths);
		return UARPG_CharacterMove_AsyncAction::SettingRequest(Character, ResultData);
	}
	return nullptr;
}

void UARPG_CharacterMove_AsyncAction::OnMoveCompletedCheckedId(FAIRequestID RequestID, const FPathFollowingResult& Result, UPathFollowingComponent* PathFollowingComponent, FAIRequestID InvokeRequestID)
{
	if (RequestID.IsEquivalent(InvokeRequestID) && Character)
	{
		PathFollowingComponent->OnRequestFinished.RemoveAll(this);
		OnMoveCompleted(Result);
	}
	else
	{
		SetReadyToDestroy();
	}
}

void UARPG_CharacterMove_AsyncAction::OnMoveCompleted(FPathFollowingResult Result)
{
	OnMoveFinished.Broadcast(Result.Code, Character);
	SetReadyToDestroy();
}

UARPG_CharacterMove_AsyncAction* UARPG_CharacterMove_AsyncAction::SettingRequest(class ACharacterBase* Character, FPathFollowingRequestResult &ResultData)
{
	UARPG_CharacterMove_AsyncAction* CharacterMove_AsyncAction = NewObject<UARPG_CharacterMove_AsyncAction>(Character);
	CharacterMove_AsyncAction->Character = Character;
	UWorld* World = Character->GetWorld();
	switch (ResultData.Code)
	{
	case EPathFollowingRequestResult::RequestSuccessful:
	{
		UPathFollowingComponent* PathFollowingComponent = UARPG_MoveUtils::GetPathFollowingComponent(Character->GetController());
		PathFollowingComponent->OnRequestFinished.AddUObject(CharacterMove_AsyncAction, &UARPG_CharacterMove_AsyncAction::OnMoveCompletedCheckedId, PathFollowingComponent, ResultData.MoveId);
	}
	break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(CharacterMove_AsyncAction, &UARPG_CharacterMove_AsyncAction::OnMoveCompleted, FPathFollowingResult(EPathFollowingResult::Success, FPathFollowingResultFlags::None)));
	}
	break;
	case EPathFollowingRequestResult::Failed:
	{
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(CharacterMove_AsyncAction, &UARPG_CharacterMove_AsyncAction::OnMoveCompleted, FPathFollowingResult(EPathFollowingResult::Invalid, FPathFollowingResultFlags::InvalidPath)));
	}
	break;
	}
	return CharacterMove_AsyncAction;
}
