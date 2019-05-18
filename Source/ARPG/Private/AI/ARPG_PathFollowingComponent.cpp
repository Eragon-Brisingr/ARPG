// Fill out your copyright notice in the Description page of Project Settings.

#include "ARPG_PathFollowingComponent.h"

void UARPG_PathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);
}

void UARPG_PathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);
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
