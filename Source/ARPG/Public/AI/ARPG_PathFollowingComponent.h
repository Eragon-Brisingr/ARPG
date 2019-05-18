// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavMeshPath.h"
#include "ARPG_PathFollowingComponent.generated.h"

/**
 * 
 */

UCLASS()
class ARPG_API UARPG_PathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

public:
	void FollowPathSegment(float DeltaTime) override;

	void SetMoveSegment(int32 SegmentStartIndex) override;

	void UpdatePathSegment() override;

public:
	void StartUsingCustomLink(INavLinkCustomInterface* CustomNavLink, const FVector& DestPoint) override;

	void FinishUsingCustomLink(INavLinkCustomInterface* CustomNavLink) override;
};
