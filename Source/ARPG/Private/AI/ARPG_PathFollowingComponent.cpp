// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_PathFollowingComponent.h"
#include "ARPG_NavAreaType.h"
#include "DrawDebugHelpers.h"
#include "CharacterBase.h"

void UARPG_PathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);
}

void UARPG_PathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];
	const FNavPathPoint& SegmentEnd = Path->GetPathPoints()[MoveSegmentStartIndex + 1];
	if (FARPG_NavAreaFlagHelper::IsSet(SegmentStart, EARPG_NavAreaFlag::Jump))
	{
		ACharacterBase* Character = Cast<ACharacterBase>(MovementComp->GetOwner());
		Character->WhenStartNavLink(EARPG_NavAreaFlag::Jump, SegmentStart.Location, SegmentEnd.Location);
	}
}

void UARPG_PathFollowingComponent::UpdatePathSegment()
{
	Super::UpdatePathSegment();
}

void UARPG_PathFollowingComponent::StartUsingCustomLink(INavLinkCustomInterface* CustomNavLink, const FVector& DestPoint)
{
	Super::StartUsingCustomLink(CustomNavLink, DestPoint);
}

void UARPG_PathFollowingComponent::FinishUsingCustomLink(INavLinkCustomInterface* CustomNavLink)
{
	Super::FinishUsingCustomLink(CustomNavLink);
}
