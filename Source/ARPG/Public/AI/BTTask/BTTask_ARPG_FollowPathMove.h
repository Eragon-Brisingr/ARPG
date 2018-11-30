// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_ARPG_FollowPathMove.generated.h"

/**
 * 
 */
struct FARPG_FollowPathMoveMemory
{
	int32 TargetPointIndex = -1;
	uint8 bMoveReverse : 1;
};

UCLASS(meta = (DisplayName = "沿路径移动"))
class ARPG_API UBTTask_ARPG_FollowPathMove : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_ARPG_FollowPathMove();

	UPROPERTY(Category = "寻路", EditAnywhere)
	TSoftObjectPtr<class AARPG_NavPath> Path;

	UPROPERTY(Category = "寻路", EditAnywhere)
	TSubclassOf<class UARPG_FollowPathMoveConfigBase> FollowPathMoveConfig;

	UPROPERTY(Category = "寻路", EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius = 5.f;

	UPROPERTY(Category = "寻路", EditAnywhere)
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FARPG_FollowPathMoveMemory); }

	void WhenMoveFinished(const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory);
};

UCLASS()
class ARPG_API UARPG_FollowPathMoveConfigBase : public UObject
{
	GENERATED_BODY()
public:
	virtual int32 GetStartFollowPathMoveIndex(class ACharacterBase* Mover, class AARPG_NavPath* Path, int32 RememberIndex) const;
};
